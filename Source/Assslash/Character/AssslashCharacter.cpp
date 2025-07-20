// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashCharacter.h"

#include "AssslashPlayerController.h"
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
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Interface/LifeComponent.h"
#include "Kismet/GameplayStatics.h"

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
	AttackTraceDistance = 100.f;

	// Collision Straint
	this->GetCapsuleComponent()->SetConstraintMode(EDOFMode::Type::YZPlane);
}

// ========== Replication ==========
void AAssslashCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AAssslashCharacter, bAttacking, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(AAssslashCharacter, bDodging, COND_SimulatedOnly);
	DOREPLIFETIME(AAssslashCharacter, Enemy);
	DOREPLIFETIME(AAssslashCharacter, LifeComponent);
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
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AAssslashCharacter::HandleAnimNotifyBegin);
		}
		
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

void AAssslashCharacter::HandleAnimNotifyBegin(FName NotifyName,
	const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UE_LOG(LogAssslash, Warning, TEXT("HandleAnimNotifyBegin : %s"), *NotifyName.ToString());
	if (NotifyName == TEXT("AN_AttackTrace"))
	{
		Server_PerformAttackTrace();
	}
	if (NotifyName == TEXT("AN_AttackEnd"))
	{
		SetIsAttacking(false);
	}
	if (NotifyName == TEXT("AN_DodgeEnd"))
	{
		SetIsDodging(false);
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

void AAssslashCharacter::Server_SetInitialRotation_Implementation()
{
	UE_LOG(LogAssslash, Log, TEXT("[SERVER] Server_SetInitialRotation_Implementation of %s"), *GetName());
	bIsLeft = 1;

	// set rotation to 180
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += 180.f;
	SetActorRotation(NewRotation);
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
		
		AddMovementInput(RightDirection, InputVector.X * (bIsLeft ? -1.f : 1.f));
	}
}

/** Attack :: Runs on Client */
void AAssslashCharacter::Attack(const FInputActionValue& ActionValue)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (bDodging == 1 || bAttacking == 1) return;
	
	float Now = GetWorld()->GetTimeSeconds();
	if (bDodging == 0  && Now - ActionLastTime > ActionInterval)
	{
		ActionLastTime = Now;
		SetIsAttacking(true);
		// AnimInstance->Montage_Play(AttackMontage, 1.f);
	}
}


/**
 * [서버에서 실행] 플레이어의 공격 ray cast에 맞았으면 실행
 * @param HitActor 
 * @param HitLocation 
 */
void AAssslashCharacter::Server_OnAttackHit(AActor* HitActor, FVector HitLocation)
{
	// cast HitActor to AAsslashCharacter
	AAssslashCharacter* HitCharacter = Cast<AAssslashCharacter>(HitActor);
	ULifeComponent* HitCharacterLifeComponent = HitCharacter->GetComponentByClass<ULifeComponent>();
	

	if (!IsValid(HitCharacter) || !IsValid(HitCharacterLifeComponent)) return;
	if (HitCharacter->bIsBusy == 1) return;

	UE_LOG(LogAssslash, Log, TEXT("[SERVER] HitCharacter : %s"), *HitCharacter->GetName());

	UDamageType* DamageTypeInstance = NewObject<UDamageType>();
	DamageTypeInstance->DamageImpulse = 10.f;
	
	HitCharacterLifeComponent->TakeDamage(
		HitCharacter,
		10,
		DamageTypeInstance,
		GetController(),
		this
		);

	// PlayerHUD->SetHealth(HitCharacter->bIsLeft, HitCharacterLifeComponent->GetHp(), HitCharacterLifeComponent->GetHpMax());
	HitCharacter->bIsBusy = 1;

	// spawn HitNiagaraSystem in every client
	if (HasAuthority())
	{
		Multicast_OnPlayerHit(HitLocation, HitCharacter);
		GetWorldTimerManager().SetTimer(
			BusyTimerHandle,
			HitCharacter,
			&AAssslashCharacter::Multicast_ResetBusyState,
			HitCharacter->ActionInterval,
			false
		);
	}
}


/**
 * [서버에서 실행] 플레이어의 공격 ray cast에 맞았으나 상대가 회피중일 때
 * @param HitActor 
 * @param HitLocation 
 */
void AAssslashCharacter::Server_OnAttackMiss(AActor* HitActor, FVector HitLocation)
{
	// cast HitActor to AAsslashCharacter
	AAssslashCharacter* HitCharacter = Cast<AAssslashCharacter>(HitActor);
	ULifeComponent* HitCharacterLifeComponent = HitCharacter->GetComponentByClass<ULifeComponent>();
	

	if (!IsValid(HitCharacter) || !IsValid(HitCharacterLifeComponent)) return;

	UE_LOG(LogAssslash, Log, TEXT("[SERVER] Miss Character : %s"), *HitCharacter->GetName());

	// spawn HitNiagaraSystem in every client
	if (HasAuthority())
	{
		//Multicast_OnPlayerHit(HitLocation, HitCharacter);
	}
}


/**
 * 내가 누군가를 때렸을 때 나한테 발생하는 Multicast
 * @param Loc 때린 위치
 */
void AAssslashCharacter::Multicast_OnPlayerHit_Implementation(FVector HitLocation, AAssslashCharacter* HitCharacter)
{
    SpawnHitEffects(HitLocation);
    HandleHitFeedback(HitCharacter);
	Server_SetWorldTimeScale(0.4f, 0.15f);

	if (HitCharacter)
	{
		FVector LaunchDirection = HitCharacter->GetActorLocation() - GetActorLocation();
		LaunchDirection.Normalize();
		LaunchDirection.Z += 0.5f;
		LaunchDirection.Normalize();

		float LaunchStrength = 500.f;

		HitCharacter->LaunchCharacter(LaunchDirection*LaunchStrength, true, true);
	}
}

void AAssslashCharacter::SpawnHitEffects(const FVector& HitLocation)
{
    constexpr float kNiagaraXOffset = -10.0f;
    constexpr float kDamageIndicatorZOffset = -70.0f;
    
    FVector NiagaraLocation = FVector(HitLocation.X + kNiagaraXOffset, HitLocation.Y, HitLocation.Z);
    FVector DamageIndicatorLocation = FVector(HitLocation.X, HitLocation.Y, HitLocation.Z + kDamageIndicatorZOffset);

    // Spawn hit VFX
    if (HitNiagaraSystem)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            HitNiagaraSystem,
            NiagaraLocation
        );
    }

	// Play explosion sound
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ExplosionSound,
			HitLocation,
			1.0f,
			1.0f,
			0.0f,
			nullptr,
			nullptr
			);
	}
	

    // Spawn damage indicator
    if (BP_DamageIndicator)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
        AActor* DamageIndicator = GetWorld()->SpawnActor<AActor>(
            BP_DamageIndicator,
            DamageIndicatorLocation,
            FRotator::ZeroRotator,
            SpawnParams
        );

        UE_LOG(LogAssslash, Log, TEXT("[MC:%s] Spawn BP_DamageIndicator at %f %f %f"), 
            *GetName(), HitLocation.X, HitLocation.Y, HitLocation.Z);
    }
}

void AAssslashCharacter::HandleHitFeedback(AAssslashCharacter* HitCharacter)
{
    if (!HasAuthority())
    {
        if (IsLocallyControlled())
        {
            // This client hit someone else
            ShakeEnemyHpBar();
        }
        else
        {
            // This client was hit by someone
            Enemy->ShakeMyHpBar();
            Enemy->ShowHitBG();
        }
    }
}

void AAssslashCharacter::ShowHitBG() const
{
	if (!PlayerHUD) return;
	PlayerHUD->PlayerShowHitBG();
}

void AAssslashCharacter::ShakeEnemyHpBar() const
{
	if (!PlayerHUD) return;
	if (bIsLeft)
	{
		PlayerHUD->PlayShakeRight();
	} else
	{
		PlayerHUD->PlayShakeLeft();
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->ClientStartCameraShake(HitCameraShakeClass, 1.0f);
	}
}

void AAssslashCharacter::ShakeMyHpBar() const
{
	if (!PlayerHUD) return;
	if (bIsLeft)
	{
		PlayerHUD->PlayShakeLeft();
	} else
	{
		PlayerHUD->PlayShakeRight();
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->ClientStartCameraShake(HitCameraShakeClass, 1.0f);
	}
}

// ========== 맞았을 때 world time scale 조정 ==========

void AAssslashCharacter::Server_SetWorldTimeScale_Implementation(float TimeScale, float ResetTime)
{
	Multicast_SetWorldTimeScale(TimeScale, ResetTime);
}

void AAssslashCharacter::Multicast_SetWorldTimeScale_Implementation(float TimeScale, float ResetTime)
{
	// 서버와 모든 클라이언트에서 실행
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeScale);
    
	// 일정 시간 후 원래 속도로 복원
	FTimerHandle TimerHandle_ResetTimeDilation;
	GetWorldTimerManager().SetTimer(
		TimerHandle_ResetTimeDilation, 
		[this]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, 
		ResetTime * UGameplayStatics::GetGlobalTimeDilation(GetWorld()), 
		false
	);
}

void AAssslashCharacter::Multicast_ResetBusyState_Implementation()
{
	bIsBusy = 0;
}

void AAssslashCharacter::Multicast_AttackAnimPlay_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackMontage, 1.f);
}

void AAssslashCharacter::Server_PerformAttackTrace_Implementation()
{
	FVector StartLocation = GetActorLocation() + GetActorRotation().RotateVector(AttackTraceOffset);
	FVector EndLocation = StartLocation + (GetActorRightVector() * AttackTraceDistance * -1);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 나는 무시하게 함.

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Pawn,
		QueryParams
	);

	#if ENABLE_DRAW_DEBUG
		DrawDebugLine(
		   GetWorld(),
		   StartLocation,
		   EndLocation,
		   bHit ? FColor::Red : FColor::Green, // 맞았으면 빨간색, 아니면 초록색
		   false, // Persistent (계속 유지) 여부
		   3.0f,  // Lifetime
		   0,     // Depth priority
		   3.0f   // Thickness
	   );
	#endif

	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		AAssslashCharacter* HitCharacter = Cast<AAssslashCharacter>(HitActor);
		check(HitCharacter);

		
		if (bHit)
		{
			if (HitCharacter->bDodging)
			{
				Server_OnAttackMiss(HitActor, HitResult.Location);
			} else
			{
				Server_OnAttackHit(HitActor, HitResult.Location);
			}
		}
	}
	
	
}

/** Attack :: Runs on Server */
void AAssslashCharacter::UpdateServerAttacking_Implementation(bool bNewAttacking)
{
	UE_LOG(LogAssslash, Log, TEXT("UpdateServerAttacking : %d %s"), bNewAttacking, *GetName());
	if (bNewAttacking == true)
	{
		Multicast_AttackAnimPlay();
		bAttacking = 1;
	} else
	{
		bAttacking = 0;
	}
}


void AAssslashCharacter::Switch(const FInputActionValue& ActionValue)
{
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

void AAssslashCharacter::SetIsAttacking(bool newAttacking)
{
	if (IsLocallyControlled())
	{
		if (newAttacking == true)
		{
			bAttacking = 1;
		} else
		{
			bAttacking = 0;
		}
		UE_LOG(LogAssslash, Log, TEXT("SetIsAttacking : %d %s"), bAttacking, *GetName());
		UpdateServerAttacking(bAttacking);
	}
}

void AAssslashCharacter::SetIsDodging(bool newDodging)
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

/** Dodging :: Runs on Client */
void AAssslashCharacter::Dodge(const FInputActionValue& ActionValue)
{
	if (bDodging == 1 || bAttacking == 1) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	float Now = GetWorld()->GetTimeSeconds();
	if (bDodging == 0  && Now - ActionLastTime > ActionInterval)
	{ 
		ActionLastTime = Now;
		SetIsDodging(true);
		
	}
}

void AAssslashCharacter::Multicast_DodgeAnimPlay_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DodgeMontage, 1.f);
}

/** Dodging :: Runs on Server */
void AAssslashCharacter::UpdateServerDodging_Implementation(bool bNewDodging)
{
	bDodging = bNewDodging;
	if (bNewDodging == true) Multicast_DodgeAnimPlay();
}


