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
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category="Enhanced Input")
	class UInputAction* SwitchAction;
	
public:
	// Sets default values for this character's properties
	AAssslashCharacter();

protected:
	/**
	 * inputs
	 **/
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Move(const FInputActionValue& ActionValue);
	void Jump(const FInputActionValue& ActionValue);
	void Switch(const FInputActionValue& ActionValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// UPROPERTY(EditAnywhere)
	// class UStaticMeshComponent* Body;
	//
	// UPROPERTY(EditAnywhere)
	// class UStaticMeshComponent* Hair;
	//
	// UPROPERTY(EditAnywhere)
	// class UStaticMeshComponent* Clothes;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class UCharacterMovementComponent* Movement;
	
	UPROPERTY(EditAnywhere)
	float MoveScale;

protected:
	/**
	 *HUD
	 **/
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UAssslashHUD> PlayerHUDClass;

	UPROPERTY()
	class UAssslashHUD* PlayerHUD;
};
