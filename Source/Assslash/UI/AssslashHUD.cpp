#include "AssslashHUD.h"

#include "EngineUtils.h"
#include "Assslash/Assslash.h"
#include "Assslash/Character/AssslashCharacter.h"
#include "Assslash/Character/Interface/LifeComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"



void UAssslashHUD::InitWithEnemy(AAssslashCharacter* Me, AAssslashCharacter* Enemy)
{
	FString MyIndex = Me->GetName().Right(1);
	FString EnemyIndex = Enemy->GetName().Right(1);
	SetHealth(MyIndex, Me->GetComponentByClass<ULifeComponent>()->GetHp(), Me->GetComponentByClass<ULifeComponent>()->GetHpMax());
	SetHealth(EnemyIndex, Enemy->GetComponentByClass<ULifeComponent>()->GetHp(), Enemy->GetComponentByClass<ULifeComponent>()->GetHpMax());
}

void UAssslashHUD::SetHealth(FString PlayerIndex, float Health, float MaxHealth)
{
	if (PlayerIndex == "0")
	{
		SetHealthLeft(Health, MaxHealth);
	} else
	{
		SetHealthRight(Health, MaxHealth);
	}
}

void UAssslashHUD::SetHealthLeft(float Health, float MaxHealth)
{
	if (LtHealthBar)
	{
		LtHealthBar->SetPercent(Health / MaxHealth);
	}
}

void UAssslashHUD::SetHealthRight(float Health, float MaxHealth)
{
	if (RtHealthBar)
	{
		RtHealthBar->SetPercent(Health / MaxHealth);
	}
}

float UAssslashHUD::GetHealthRight()
{
	return HealthRight;
}

float UAssslashHUD::GetHealthLeft()
{
	return HealthLeft;
}


