// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	
};
