// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashUIGameMode.h"

#include "HeadMountedDisplayTypes.h"
#include "Assslash/Assslash.h"
#include "Assslash/Character/AssslashPlayerController.h"

void AAssslashUIGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogAssslash, Log, TEXT("[UIGAMEMODE] BeginPlay"));
}

void AAssslashUIGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogAssslash, Log, TEXT("[UIGAMEMODE] PostLogin"));

	if (NewPlayer)
	{
	} else
	{
		UE_LOG(LogAssslash, Error, TEXT("[UIGAMEMODE] PlayerController is not found!"));
	}
}
