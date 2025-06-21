// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "BattleCam.generated.h"

UCLASS()
class ASSSLASH_API ABattleCam : public ACameraActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABattleCam();

	UPROPERTY(EditAnywhere, Category="카메라")
	float CamFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="카메라 플레이어 간 이동")
	float CamZOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="카메라 플레이어 간 이동")
	float CamYOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="카메라 플레이어 간 이동")
	float CamXOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="카메라 플레이어 간 이동")
	float InterpSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector TargetLocation;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FRotator CalculateLookRotation(FVector TargetLocation);
	
};
