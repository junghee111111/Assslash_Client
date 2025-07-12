// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashCharacter.h"

#include "AssslashPlayerController.h"
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Assslash/Assslash.h"
#include "Assslash/Common/AssslashGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Assslash/UI/AssslashHUD.h"
#include "Behaviour/AssslashCharacterAttackBoundary.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameStateBase.h"
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
	ActionInterval = .5f;
	SpawnedAttackBoundary = nullptr;

	// Collision Straint
	this->GetCapsuleComponent()->SetConstraintMode(EDOFMode::Type::YZPlane);
}

// Called when the game starts or when spawned
void AAssslashCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (GetLocalRole() == ROLE_Authority)
	{
		// init HUD
		if (AAssslashGameMode* GameMode = Cast<AAssslashGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->OnPlayerConnected.AddDynamic(this, &AAssslashCharacter::HandleNewPlayerConnected);
			GameMode->OnPlayerPawnReady.AddDynamic(this, &AAssslashCharacter::HandleNewPlayerReady);
		}
	}
	if (IsLocallyControlled())
	{
		int32 CurrentTotalPlayersNum = 0;
		UWorld* World = GetWorld();
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


	if (PlayerHUD == nullptr && IsValid(Enemy) && IsLocallyControlled() && PlayerHUDClass)
	{
		ShowHUD(GetController<AAssslashPlayerController>());
	}
	
	if (bAttacking && !SpawnedAttackBoundary)
	{
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

	if (bAttacking || bDodging)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	} else
	{
		if (GetCharacterMovement()->MaxWalkSpeed <= 1)
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
	DOREPLIFETIME_CONDITION(AAssslashCharacter, bDodging, COND_SimulatedOnly);
	DOREPLIFETIME(AAssslashCharacter, Enemy);
}

void AAssslashCharacter::ShowHUD(AAssslashPlayerController* APC)
{
	PlayerHUD = CreateWidget<UAssslashHUD>(APC, PlayerHUDClass);
	if (!PlayerHUD) return;
	if (!IsValid(Enemy))
	{
		UE_LOG(LogAssslash, Error, TEXT("[%s] enemy is not found!"), *GetName())
		return;
	}
	PlayerHUD->AddToPlayerScreen();
	PlayerHUD->InitWithEnemy(this, Enemy);
	UE_LOG(LogAssslash, Error, TEXT("[%s] Display HUD Correctly!"), *GetName())
}

void AAssslashCharacter::Multicast_DisplayHUD_Implementation()
{
	if (!IsLocallyControlled())
	{
		UE_LOG(LogAssslash, Error, TEXT("[MC:%s] Not locally controlled, skipping HUD creation."), *GetName());
		return; 
	}
    
	AAssslashPlayerController* APC = GetController<AAssslashPlayerController>();
	if (APC && PlayerHUDClass && PlayerHUD == nullptr)
	{
		ShowHUD(APC);
	}
	else if (PlayerHUD && PlayerHUD->IsInViewport())
	{
		UE_LOG(LogAssslash, Log, TEXT("[MC:%s] HUD already exists and is in viewport. Not re-creating."), *GetName());
	}
	else if (PlayerHUD && !PlayerHUD->IsInViewport())
	{
		PlayerHUD->AddToPlayerScreen();
		UE_LOG(LogAssslash, Log, TEXT("[MC:%s] Existing HUD re-added to screen."), *GetName());
	}
	else if (!PlayerHUDClass)
	{
		UE_LOG(LogAssslash, Error, TEXT("[MC:%s] PlayerHUDClass is not set! Please assign it in Blueprint."), *GetName());
	}
	
}


void AAssslashCharacter::HandleNewPlayerReady(APlayerController* NewPlayerController)
{
	// UE_LOG(LogAssslash, Log, TEXT("[%s] HandleNewPlayerReady : %s"),*GetName() ,*NewPlayerController->GetName());
}

void AAssslashCharacter::HandleNewPlayerConnected(APlayerController* NewPlayerController)
{
	UWorld* World = GetWorld();
	int32 CurrentTotalPlayersNum = World->GetNumPlayerControllers();
	int32 CurrentPlayerStatesNum = World->GetGameState()->PlayerArray.Num();
	//UE_LOG(LogAssslash, Log, TEXT("[SERVER] HandleNewPlayerConnected : %s (c : %d) (s : %d)"), *NewPlayerController->GetName(), CurrentTotalPlayersNum,CurrentPlayerStatesNum);
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
	if (bDodging == 1 || bAttacking == 1) return;
	
	float Now = GetWorld()->GetTimeSeconds();
	if (bDodging == 0  && Now - ActionLastTime > ActionInterval)
	{
		ActionLastTime = Now;
		bAttacking = 1;
		UpdateServerAttacking(true);
	}
}

/** Attack :: Runs on Server */
void AAssslashCharacter::UpdateServerAttacking_Implementation(bool bNewAttacking)
{
	UE_LOG(LogAssslash, Log, TEXT("UpdateServerAttacking : %d %s"), bNewAttacking, *GetName());
	if (bNewAttacking == true)
	{
		bAttacking = 1;
	} else
	{
		bAttacking = 0;
	}
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

void AAssslashCharacter::SetEnemy(AAssslashCharacter* NewEnemy)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Enemy = NewEnemy;
		UE_LOG(LogAssslash, Log, TEXT("[SERVER] %s's Enemy set to %s."), *GetName(), *Enemy->GetName());
	}
}

bool AAssslashCharacter::GetIsAttacking()
{
	if (bAttacking == 1)
	{
		return true;
	}
	return false;
}

bool AAssslashCharacter::GetIsDodging()
{
	if (bDodging == 1)
	{
		return true;
	}
	return false;
}

void AAssslashCharacter::SetIsDodging(bool newDodging)
{
	if (IsLocallyControlled())
	{
		if (newDodging == true)
		{
			bDodging = 1;
		} else
		{
			bDodging = 0;
		}
		UE_LOG(LogAssslash, Log, TEXT("SetIsDodging : %d %s"), bDodging, *GetName());
		UpdateServerDodging(bDodging);
	}
}

/** Dodging :: Runs on Client */
void AAssslashCharacter::Dodge(const FInputActionValue& ActionValue)
{
	float Now = GetWorld()->GetTimeSeconds();
	if (bDodging == 1 || bAttacking == 1) return;
	if (bDodging == 0  && Now - ActionLastTime > ActionInterval)
	{
		ActionLastTime = Now;
		SetIsDodging(true);
		UE_LOG(LogAssslash, Log, TEXT("Dodge : %s"), *GetName());
	}
}

/** Dodging :: Runs on Server */
void AAssslashCharacter::UpdateServerDodging_Implementation(bool bNewDodging)
{
	bDodging = bNewDodging;
}


