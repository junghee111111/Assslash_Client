// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AssslashGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API AAssslashGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAssslashGameMode();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
};
