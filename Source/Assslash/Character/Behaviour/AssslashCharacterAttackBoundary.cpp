// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashCharacterAttackBoundary.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AAssslashCharacterAttackBoundary::AAssslashCharacterAttackBoundary()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->SetSphereRadius(AttackCollisionRadius);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	AttackNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System"));
	AttackNiagaraSystem->SetupAttachment(Collision);
}

// Called when the game starts or when spawned
void AAssslashCharacterAttackBoundary::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAssslashCharacterAttackBoundary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

