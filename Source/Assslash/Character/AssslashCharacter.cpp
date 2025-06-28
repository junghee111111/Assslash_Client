// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashCharacter.h"

#include "AssslashPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Assslash/Assslash.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Assslash/UI/AssslashHUD.h"
#include "Behaviour/AssslashCharacterAttackBoundary.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAssslashCharacter::AAssslashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	// SetRootComponent(Capsule);

	// Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	// Body->SetupAttachment(RootComponent);
	//
	// Hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	// Hair->SetupAttachment(Body);
	//
	// Clothes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Clothes"));
	// Clothes->SetupAttachment(Body);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	// Camera->bUsePawnControlRotation = true;
	
	// Movement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("Movement"));
	MoveScale = 1.f;
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// HUD
	PlayerHUDClass = nullptr;
	PlayerHUD = nullptr;

	// Attack
	AttackOffsetAdjustment = FVector(0.f, 0.f, 0.f);
	AttackClass = AAssslashCharacterAttackBoundary::StaticClass();
	AttackInterval = .3f;
	SpawnedAttackBoundary = nullptr;

	// Collision Straint
	this->GetCapsuleComponent()->SetConstraintMode(EDOFMode::Type::YZPlane);
}

// Called when the game starts or when spawned
void AAssslashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled() && PlayerHUDClass)
	{
		AAssslashPlayerController* APC = GetController<AAssslashPlayerController>();
		check(APC);

		PlayerHUD = CreateWidget<UAssslashHUD>(APC, PlayerHUDClass);
		check(PlayerHUD);

		PlayerHUD->AddToPlayerScreen();
	}
}

void AAssslashCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}
}


// Called every frame
void AAssslashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float Now = GetWorld()->GetTimeSeconds();

	if (bAttacking && Now - AttackLastTime > AttackInterval && !SpawnedAttackBoundary)
	{
		AttackLastTime = Now;
		FTransform Transform = GetActorTransform();
		FRotator Rotation = Transform.Rotator();
		FVector Translation = Transform.GetTranslation() + Rotation.RotateVector(AttackOffsetAdjustment);
	
		SpawnedAttackBoundary = GetWorld()->SpawnActor<AAssslashCharacterAttackBoundary>(AttackClass, Translation, Rotation);
		if (SpawnedAttackBoundary)
		{
			if (IsLocallyControlled())
			{
				SpawnedAttackBoundary->SetIsLocal(true);
				SpawnedAttackBoundary->OnLocalAttackBoundaryFinished.AddDynamic(this, &AAssslashCharacter::OnLocalAttackBoundaryCompleted);
			} else
			{
				SpawnedAttackBoundary->OnRemoteAttackBoundaryFinished.AddDynamic(this, &AAssslashCharacter::OnRemoteAttackBoundaryCompleted);
			}
			
		}
	}

	if (bAttacking)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	} else
	{
		if (GetCharacterMovement()->MaxWalkSpeed <= 10)
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		}
	}
}


// Called to bind functionality to input
void AAssslashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	AAssslashPlayerController* APC = Cast<AAssslashPlayerController>(Controller);
	
	// get local player
	ULocalPlayer* LocalPlayer = APC->GetLocalPlayer();
	check(LocalPlayer);

	// subsystem attached to local player
	UEnhancedInputLocalPlayerSubsystem* subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EIC);

	// bind actions
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAssslashCharacter::Move);
	EIC->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &AAssslashCharacter::Switch);
	EIC->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AAssslashCharacter::Attack);
	EIC->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AAssslashCharacter::Dodge);
	EIC->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &AAssslashCharacter::Switch);
	
	UE_LOG(LogAssslash, Log, TEXT("PlayerPawn SetupPlayerInputComponent Done!"));
}

void AAssslashCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AAssslashCharacter, bAttacking, COND_SimulatedOnly);
}

void AAssslashCharacter::Move(const struct FInputActionValue& InputValue)
{
	
	FVector2D InputVector = InputValue.Get<FVector2D>();
	
	if (IsValid(Controller))
	{
		// UE_LOG(LogAssslash, Log, TEXT("Move : %f %f"), InputVector.X, InputVector.Y);
		// move right to side scroller  2d
		const FVector RightDirection = GetActorRightVector();
		
		AddMovementInput(RightDirection, InputVector.X);
	}
}

/** Attack :: Runs on Client */
void AAssslashCharacter::Attack(const FInputActionValue& ActionValue)
{
	if (!bAttacking) bAttacking = true;

	UpdateServerAttacking(bAttacking);
}

/** Attack :: Runs on Server */
void AAssslashCharacter::UpdateServerAttacking_Implementation(bool bNewAttacking)
{
	bAttacking = bNewAttacking;
}

void AAssslashCharacter::Dodge(const FInputActionValue& ActionValue)
{
}

void AAssslashCharacter::Switch(const FInputActionValue& ActionValue)
{
}

void AAssslashCharacter::OnLocalAttackBoundaryCompleted()
{
	if (IsLocallyControlled())
	{
		bAttacking = false;
		SpawnedAttackBoundary = nullptr;
	}
}

void AAssslashCharacter::OnRemoteAttackBoundaryCompleted()
{
	if (!IsLocallyControlled()){
		bAttacking = false;
		SpawnedAttackBoundary = nullptr;
	}
}

bool AAssslashCharacter::GetIsAttacking()
{
	return bAttacking;
}
