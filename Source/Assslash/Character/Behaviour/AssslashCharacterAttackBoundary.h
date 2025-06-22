// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssslashCharacterAttackBoundary.generated.h"

UCLASS()
class ASSSLASH_API AAssslashCharacterAttackBoundary : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAssslashCharacterAttackBoundary();


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* AttackNiagaraSystem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
