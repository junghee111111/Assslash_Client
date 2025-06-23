// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "AssslashCharacter.generated.h"

UCLASS()
class ASSSLASH_API AAssslashCharacter : public ACharacter
{
	GENERATED_BODY()
	
protected:
	// inputs
	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	class UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	class UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	class UInputAction* SwitchAction;

	UPROPERTY(EditAnywhere, Category="Assslash Attack")
	FVector AttackOffsetAdjustment;

	UPROPERTY(EditAnywhere, Category="Assslash Attack")
	TSubclassOf<class AAssslashCharacterAttackBoundary> AttackClass;

public:
	// Sets default values for this character's properties
	AAssslashCharacter();

	/** Override to set up replicated properties */
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Handle input to update location. */
	void Move(const FInputActionValue& ActionValue);

	/** Handle input to start attacking. */
	void Attack(const FInputActionValue& ActionValue);

	/** Handle input to start dodging. */
	void Dodge(const FInputActionValue& ActionValue);

	/** Handle input to change between 2 characters. (like tekken tag) */
	void Switch(const FInputActionValue& ActionValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class UCharacterMovementComponent* Movement;
	
	UPROPERTY(EditAnywhere)
	float MoveScale;

protected:
	/** HUD */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAssslashHUD> PlayerHUDClass;

	UPROPERTY()
	class UAssslashHUD* PlayerHUD;

	/** Behaviours */
	UPROPERTY(Replicated)
	uint32 bAttacking:1;

	UPROPERTY(EditAnywhere, Category="Assslash Attack")
	uint32 AttackInterval;

private:
	float AttackLastTime;

	UPROPERTY(Transient, meta = (AllowPrivateAccess = "true"))
	AAssslashCharacterAttackBoundary* SpawnedAttackBoundary;
	
	UFUNCTION()
	void OnAttackBoundaryCompleted();
	
public:
	/** rpc functions*/
	UFUNCTION(Server, Reliable)
	void UpdateServerAttacking(bool bNewAttacking);
};
