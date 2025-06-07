// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashGameMode.h"

#include "Assslash/Assslash.h"

AAssslashGameMode::AAssslashGameMode()
{

}

void AAssslashGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogAssslash, Log, TEXT("Game is running : %s %s"), *MapName, *Options)
}
