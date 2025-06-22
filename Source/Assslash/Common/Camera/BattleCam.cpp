// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCam.h"

#include "Assslash/Assslash.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"


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
	int32 CamXAdjust = 0;

	// 모든 PlayerState를 불러와서 Actor Location에 접근한다.
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

	// 플레이어가 2명이라면 서로의 거리를 계산하여 CamXAdjust 값을 도출해낸다.
	if (ValidPlayerCount == 2)
	{
		int32 distance = FVector::Dist(PlayerStates[0]->GetPawn()->GetActorLocation(), PlayerStates[1]->GetPawn()->GetActorLocation());
		CamXAdjust = distance / -3;
	}

	// 플레이어가 한명이라도 있으면 위치 조정 함수 실행
	// 멀티플레이어 연결 시도 직후 상태라면 Prelogin 함수가 호출되기 전까지 Player가 0명이므로
	// 이에 대한 예외 처리가 필요하기 때문에 아래의 If문으로 한번 보호해줘야한다.
	if (ValidPlayerCount > 0) UpdateCamPositionAndRotation(SumLocation, ValidPlayerCount, CamXAdjust, DeltaTime);
}

/**
 * 여러명의 플레이어의 위치 Vector를 계산하여 카메라를 1:1 대전 게임 형식에 어울리도록
 * 위치 및 방향을 부드럽게 조정한다.
 * @param SumLoc 모든 플레이어 Position Vector의 합
 * @param ValidPlayerCount 모든 플레이어 수
 * @param CamXAdjust Camera X position의 런타임 조정 값
 * @param DeltaTime Tick 간 Delta time
 */
void ABattleCam::UpdateCamPositionAndRotation(FVector SumLoc, int32 ValidPlayerCount, int32 CamXAdjust, float DeltaTime)
{
	FVector AvgLoc = SumLoc / ValidPlayerCount;
	TargetLocation.X = AvgLoc.X + CamXOffset + CamXAdjust;
	TargetLocation.Y = AvgLoc.Y + CamYOffset;
	TargetLocation.Z = AvgLoc.Z + CamZOffset;
		
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, InterpSpeed));

	FRotator CalculatedLookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AvgLoc);
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), CalculatedLookRotation , DeltaTime, InterpSpeed));
	
}


