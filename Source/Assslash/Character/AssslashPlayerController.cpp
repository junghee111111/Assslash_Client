// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashPlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Assslash/Assslash.h"

void AAssslashPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	bAutoManageActiveCameraTarget = false;
}

void AAssslashPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (BattleCamActorClass)
	{
		BattleCamActor = GetWorld()->SpawnActor<ABattleCam>(BattleCamActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (BattleCamActor)
		{
			SetViewTargetWithBlend(BattleCamActor);
		} else
		{
			UE_LOG(LogAssslash, Error, TEXT("BattleCamActorClass가 정의되어 있지만 BattleCamActor를 Spawn하지 못하였습니다."));
		}
	} else
	{
		UE_LOG(LogAssslash, Error, TEXT("BattleCamActorClass가 정의되지 않았습니다!"))
	}
}

void AAssslashPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AActor* CurrentViewTarget = GetViewTarget();
	// UE_LOG(LogAssslash, Warning, TEXT("Current ViewTarget: %s (Class: %s)"), *CurrentViewTarget->GetName(), *CurrentViewTarget->GetClass()->GetName());
}


