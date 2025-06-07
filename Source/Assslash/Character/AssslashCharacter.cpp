// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashCharacter.h"

#include "AssslashPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Assslash/Assslash.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AAssslashCharacter::AAssslashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	// SetRootComponent(Capsule);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);

	Hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(Body);

	Clothes = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Clothes"));
	Clothes->SetupAttachment(Body);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Camera->bUsePawnControlRotation = true;
	
	// Movement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("Movement"));
	MoveScale = 1.f;

}

// Called when the game starts or when spawned
void AAssslashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAssslashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	// EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AAssslashCharacter::Jump);
	EIC->BindAction(SwitchAction, ETriggerEvent::Triggered, this, &AAssslashCharacter::Switch);
	UE_LOG(LogAssslash, Log, TEXT("PlayerPawn SetupPlayerInputComponent Done!"));
}

void AAssslashCharacter::Move(const struct FInputActionValue& InputValue)
{
	
	FVector2D InputVector = InputValue.Get<FVector2D>();
	
	if (IsValid(Controller))
	{
		UE_LOG(LogAssslash, Log, TEXT("Move : %f %f"), InputVector.X, InputVector.Y);
		// move right to side scroller  2d
		const FVector RightDirection = GetActorRightVector();
		
		AddMovementInput(RightDirection, InputVector.X);
	}
}

void AAssslashCharacter::Switch(const FInputActionValue& ActionValue)
{
}

