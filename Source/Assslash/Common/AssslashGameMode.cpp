// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashGameMode.h"

#include "EngineUtils.h"
#include "ToolBuilderUtil.h"
#include "Assslash/Assslash.h"
#include "Assslash/Character/AssslashCharacter.h"
#include "Assslash/Character/AssslashPlayerController.h"
#include "GameFramework/PlayerStart.h"

AAssslashGameMode::AAssslashGameMode()
{
	PlayerControllerClass = AAssslashPlayerController::StaticClass();
	DefaultPawnClass = AAssslashCharacter::StaticClass();
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

	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}
