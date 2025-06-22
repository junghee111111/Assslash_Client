// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashCharacterAttackBoundary.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Assslash/Assslash.h"

// Sets default values
AAssslashCharacterAttackBoundary::AAssslashCharacterAttackBoundary()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// setting up collision
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
	Collision->SetCollisionProfileName(TEXT("OverlapAll"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAssslashCharacterAttackBoundary::OnOverlap);

	AttackNiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara System"));
	AttackNiagaraSystem->SetupAttachment(Collision);
}

void AAssslashCharacterAttackBoundary::OnOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogAssslash, Log, TEXT("Hit Something"));
	Destroy();
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

