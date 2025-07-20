// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashGameMode.h"

#include "EngineUtils.h"
#include "ToolBuilderUtil.h"
#include "Assslash/Assslash.h"
#include "Assslash/Character/AssslashCharacter.h"
#include "Assslash/Character/AssslashPlayerController.h"
#include "Assslash/Character/Interface/LifeComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


AAssslashGameMode::AAssslashGameMode()
{
	PlayerControllerClass = AAssslashPlayerController::StaticClass();
	DefaultPawnClass = AAssslashCharacter::StaticClass();

	// Audio Component Init.
	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	BGMComponent->SetupAttachment(RootComponent);
	BGMComponent->bAutoActivate = true;
}



void AAssslashGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogAssslash, Log, TEXT("Game is running : %s %s"), *MapName, *Options)

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		FreePlayerStarts.Add(*It);
		UE_LOG(LogAssslash, Log, TEXT("Found player start : %s"), *(*It)->GetName())
	}
}

void AAssslashGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	if (FreePlayerStarts.Num() == 0)
	{
		ErrorMessage = TEXT("No free player starts");
	}
	
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

FString AAssslashGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
                                         const FString& Options, const FString& Portal)
{
	if (FreePlayerStarts.Num() == 0)
	{
		UE_LOG(LogAssslash, Error, TEXT("No free player starts @ InitNewPlayer"));
		return FString(TEXT("No free player starts"));
	}

	NewPlayerController->StartSpot = FreePlayerStarts.Pop();
	UE_LOG(LogAssslash, Log, TEXT("New player '%s' start : %s"),*NewPlayerController->GetName(), *NewPlayerController->StartSpot->GetName())

	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	if (Result.IsEmpty())
	{
		// OnPlayerConnected Delegate 호출한다.
		OnPlayerConnected.Broadcast(NewPlayerController);
	}
	
	return Result;
}

void AAssslashGameMode::HandleAllPlayersReady()
{
	AAssslashCharacter* Player1Char = Cast<AAssslashCharacter>(ReadyPlayers[0]->GetPawn());
	AAssslashCharacter* Player2Char = Cast<AAssslashCharacter>(ReadyPlayers[1]->GetPawn());
	
	

	if (IsValid(Player1Char) && IsValid(Player2Char))
	{
		// 서로를 Enemy로 설정 (서버에서만 실행됨)
		Player1Char->SetEnemy(Player2Char); 
		Player2Char->SetEnemy(Player1Char);
		Player2Char->Server_SetInitialRotation();
		UE_LOG(LogAssslash, Log, TEXT("[GAMEMODE] Set enemies. %s is enemy of %s, and vice versa."), *Player1Char->GetName(), *Player2Char->GetName());
	}
}

void AAssslashGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);
	UE_LOG(LogAssslash, Log, TEXT("[GAMEMODE] PostLogin : %s"), *NewPlayerController->GetName());
	
	APlayerController* PC = Cast<APlayerController>(NewPlayerController);
	if (NewPlayerController && NewPlayerController->GetPawn())
	{
		if (!ReadyPlayers.Contains(NewPlayerController))
		{
			ReadyPlayers.Add(PC);
			OnPlayerPawnReady.Broadcast(PC);
			if (ReadyPlayers.Num() == 2)
			{
				HandleAllPlayersReady();
			}
		}
	}
}

void AAssslashGameMode::RestartPlayer(AController* NewPlayerController)
{
	Super::RestartPlayer(NewPlayerController);
	UE_LOG(LogAssslash, Log, TEXT("[GAMEMODE] RestartPlayer : %s"), *NewPlayerController->GetName())
	
}

void AAssslashGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (ReadyPlayers.Num() < 2) return;
	
	
	AAssslashCharacter* Player1Char = Cast<AAssslashCharacter>(ReadyPlayers[0]->GetPawn());
	AAssslashCharacter* Player2Char = Cast<AAssslashCharacter>(ReadyPlayers[1]->GetPawn());
	
	if (IsValid(Player1Char) && IsValid(Player2Char))
	{
		UE_LOG(LogAssslash, Log, TEXT("[GAMEMODE] Tick : %s"), *Player1Char->GetName());
		if (Player1Char->GetComponentByClass<ULifeComponent>()->GetHp()<=0 ||
			Player2Char->GetComponentByClass<ULifeComponent>()->GetHp()<=0
		)
		{
			Player1Char->Client_PlayKOAnim();
			Player2Char->Client_PlayKOAnim();
		}
	}
}
