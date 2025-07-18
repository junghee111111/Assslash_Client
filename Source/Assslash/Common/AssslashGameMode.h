// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AssslashGameMode.generated.h"

/**
 * 
 */

// OnPlayerConnected Delegate 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerConnected, APlayerController*, NewPlayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerPawnReady, APlayerController*, NewPlayer);

UCLASS()
class ASSSLASH_API AAssslashGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AAssslashGameMode();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

	UPROPERTY(BlueprintAssignable, Category="Networking")
	FOnPlayerPawnReady OnPlayerPawnReady;

	UPROPERTY(BlueprintAssignable, Category="Networking")
	FOnPlayerConnected OnPlayerConnected;
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;

private:
	TArray<class APlayerStart*> FreePlayerStarts;

	UPROPERTY()
	TArray<APlayerController*> ReadyPlayers;

	UFUNCTION()
	void HandleAllPlayersReady();
};
