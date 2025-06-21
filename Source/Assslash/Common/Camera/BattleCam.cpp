// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCam.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"


// Sets default values
ABattleCam::ABattleCam()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CamXOffset = -200.0f;
	CamYOffset = 0.0f;
	CamZOffset = -50.0f;
	InterpSpeed = 5.0f;
}

// Called when the game starts or when spawned
void ABattleCam::BeginPlay()
{
	Super::BeginPlay();
	GetCameraComponent()->SetFieldOfView(CamFOV);
	TargetLocation = GetActorLocation();
}

// Called every frame
void ABattleCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGameStateBase* GameState = GetWorld()->GetGameState();

	TArray<APlayerState*> PlayerStates = GameState->PlayerArray;
	FVector SumLocation = FVector::ZeroVector;
	int32 ValidPlayerCount = 0;

	for (APlayerState* pState : PlayerStates)
	{
		if (pState)
		{
			APawn* pPawn = pState->GetPawn();
			if (!pPawn) continue;
			SumLocation += pPawn->GetActorLocation();
			ValidPlayerCount ++;
		}
	}

	if (ValidPlayerCount > 0)
	{
		FVector AvgLoc = SumLocation / ValidPlayerCount;
		TargetLocation.X = AvgLoc.X;
		TargetLocation.Y = AvgLoc.Y;
		TargetLocation.Z = AvgLoc.Z;
		
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed));
	}
}

