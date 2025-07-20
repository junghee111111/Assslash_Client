// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "AssslashPlayerController.h"
#include "Assslash/UI/Indicator/InfoIndicator.h"
#include "AssslashCharacter.generated.h"

class UAnimMontage;
class UAnimInstance;

UCLASS()
class ASSSLASH_API AAssslashCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	// ========== inputs ==========
	UPROPERTY(EditAnywhere, Category="Assslash|Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category="Assslash|Enhanced Input")
	class UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Assslash|Enhanced Input")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category="Assslash|Enhanced Input")
	class UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category="Assslash|Enhanced Input")
	class UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, Category="Assslash|UI")
	TSubclassOf<class AInfoIndicator> BP_DamageIndicator;

	UPROPERTY(EditAnywhere, Category="Assslash|UI")
	TSubclassOf<class AInfoIndicator> BP_DamageIndicator_Critical;

	UPROPERTY(EditAnywhere, Category="Assslash|UI")
	TSubclassOf<class AInfoIndicator> BP_InfoIndicator;

	UPROPERTY(EditAnywhere, Category="Assslash|Attack")
	FVector AttackOffsetAdjustment;

	UPROPERTY(EditAnywhere, Category="Assslash|Attack")
	TSubclassOf<class AAssslashCharacterAttackBoundary> AttackClass;

public:
	// Sets default values for this character's properties
	AAssslashCharacter();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisplayHUD();

	/** Override to set up replicated properties */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// ======================================================
	// Delegates

	// UPROPERTY(BlueprintAssignable, Category="Assslash|Damage")
	// FTakeAnyDamageSignature OnTakeAnyDamage;

	// ========== Sounds ==========
	// 폭발 사운드 에셋
	UPROPERTY(EditAnywhere, Category = "Assslash|SFX")
	USoundBase* ExplosionSound;


protected:
	UFUNCTION()
	void ShowHUD(AAssslashPlayerController* APC);
	
	
	UFUNCTION()
	void HandleNewPlayerConnected(APlayerController* NewPlayerController);
	
	UFUNCTION()
	void HandleNewPlayerReady(APlayerController* NewPlayerController);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandleAnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	
	/** Handle input to start dodging. */
	void Dodge(const FInputActionValue& ActionValue);

	/** Handle input to change between 2 characters. (like tekken tag) */
	void Switch(const FInputActionValue& ActionValue);

	
protected:
	/** Handle input to update location. */
	void Move(const FInputActionValue& ActionValue);
	
	//  ========== :: Attack :: ==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack|Animation")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	float AttackTraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack")
	FVector AttackTraceOffset;

	UPROPERTY(Replicated)
	uint8 bAttacking:1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack|FX")
	class UNiagaraSystem* HitNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attack|FX")
	class UNiagaraSystem* HitCriticalNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;
	
	void Attack(const FInputActionValue& ActionValue);

	UFUNCTION(Server, Reliable)
	void Server_PerformAttackTrace();

	void Server_OnAttackMiss(AActor* HitActor, FVector HitLocation);
	void SpawnHitEffects(const FVector& HitLocation);
	void HandleHitFeedback(AAssslashCharacter* HitCharacter);

	void Server_OnAttackHit(AActor* HitActor, FVector HitLocation);

	void ShowHitBG() const;
	void ShakeEnemyHpBar() const;
	void ShakeMyHpBar() const;

	UFUNCTION(Server, Reliable)
	void UpdateServerAttacking(bool bNewAttacking);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttackAnimPlay();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnPlayerHit(FVector Loc, AAssslashCharacter* HitCharacter);

	UFUNCTION(Server, Reliable)
	void Server_SetWorldTimeScale(float TimeScale, float ResetTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetWorldTimeScale(float TimeScale, float ResetTime);

	//  ========== :: Dodge :: ==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack|Animation")
	UAnimMontage* DodgeMontage;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DodgeAnimPlay();

	//  ========== :: Switch :: ==========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack|Animation")
	UAnimMontage* SwitchMontage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(Replicated)
	class ULifeComponent* LifeComponent;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class UCharacterMovementComponent* Movement;
	
	UPROPERTY(EditAnywhere)
	float MoveScale;

	UFUNCTION(Server, Reliable)
	void Server_SetInitialRotation();

protected:
	/** HUD */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAssslashHUD> PlayerHUDClass;

	UPROPERTY()
	class UAssslashHUD* PlayerHUD;

	/** Behaviours */

	UPROPERTY(EditAnywhere, Category="Assslash|Attack")
	float ActionInterval;

	UPROPERTY(Replicated)
	uint8 bDodging:1 = 0;

	UPROPERTY(Replicated)
	uint8 bIsBusy:1 = 0;

	UPROPERTY(Replicated)
	uint8 bIsLeft:1 = 0;
	
	FTimerHandle BusyTimerHandle;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResetBusyState();
	

private:
	float ActionLastTime;

	UPROPERTY(Transient, meta = (AllowPrivateAccess = "true"))
	AAssslashCharacterAttackBoundary* SpawnedAttackBoundary;

	float MaxWalkSpeed = 500.0f;
	
	
public:
	UPROPERTY(Replicated)
	AAssslashCharacter* Enemy;
	
	void SetEnemy(AAssslashCharacter* NewEnemy);

	bool GetIsLeft() const
	{
		return bIsLeft;
	}

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();

	UFUNCTION(Server, Reliable)
	void UpdateServerDodging(bool bNewDodging);

	UFUNCTION(BlueprintCallable)
	bool GetIsDodging();
	void SetIsAttacking(bool newAttacking);

	UFUNCTION(BlueprintCallable)
	void SetIsDodging(bool newDodging);
};