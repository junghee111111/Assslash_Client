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
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;

private:
	TArray<class APlayerStart*> FreePlayerStarts;
};
