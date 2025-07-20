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
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintAssignable, Category="Networking")
	FOnPlayerPawnReady OnPlayerPawnReady;

	UPROPERTY(BlueprintAssignable, Category="Networking")
	FOnPlayerConnected OnPlayerConnected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* BGMComponent;
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;

// ========== Audio Play ==========
protected:
    
	// 현재 재생 중인 BGM 추적
	UPROPERTY()
	USoundBase* CurrentBGM;

	// 현재 재생 중인 BGM 추적
	UPROPERTY()
	USoundBase* BGMCue;
    
	// BGM 볼륨
	UPROPERTY(EditAnywhere, Category = "Audio")
	float BGMVolume =1.0f;
    
	// BGM 페이드 인 시간 (초)
	UPROPERTY(EditAnywhere, Category = "Audio")
	float FadeInTime = 1.0f;
    
	// BGM을 찾는 경로 (기본값 설정)
	UPROPERTY(EditAnywhere, Category = "Audio")
	FString BGMBasePath = "/Game/Data/Sound/BGM/";

	
private:
	TArray<class APlayerStart*> FreePlayerStarts;

	UPROPERTY()
	TArray<APlayerController*> ReadyPlayers;

	UFUNCTION()
	void HandleAllPlayersReady();
};