// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeComponent.h"

#include "Assslash/Assslash.h"
#include "Assslash/Character/AssslashCharacter.h"


// Sets default values for this component's properties
ULifeComponent::ULifeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	HpMax = 100;
	Hp = HpMax;

	// ...
}

float ULifeComponent::GetHp() const
{
	return Hp;
}

float ULifeComponent::GetHpMax() const
{
	return HpMax;
}

// Called when the game starts
void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &ULifeComponent::TakeDamage);
	}
	
}

void ULifeComponent::TakeDamage(
	AActor* DamagedActor,
	float Damage,
	const class UDamageType* DamageType,
	class AController* InstigatedBy,
	AActor* DamageCauser
	)
{
	if (Damage <= 0) return;
	Hp = FMath::Clamp((Hp - Damage), 0.f, HpMax);

	AAssslashCharacter* Owner = Cast<AAssslashCharacter>(GetOwner());
	OnHealthChanged.Broadcast(Hp, HpMax, Owner->GetIsLeft());
	// UE_LOG(LogAssslash, Error, TEXT("TakeDamage : %f, Hp : %f"), Damage, Hp);
}


// Called every frame
// void ULifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
// 	// ...
// }
