// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assslash/Common/Camera/BattleCam.h"
#include "GameFramework/PlayerController.h"
#include "AssslashPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API AAssslashPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="카메라 세팅")
	TSubclassOf<ABattleCam> BattleCamActorClass;

	UPROPERTY()
	ABattleCam* BattleCamActor;
};
