#include "AssslashHUD.h"

#include "EngineUtils.h"
#include "Assslash/Assslash.h"
#include "Assslash/Character/AssslashCharacter.h"
#include "Assslash/Character/Interface/LifeComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"



void UAssslashHUD::InitWithEnemy(AAssslashCharacter* Me, AAssslashCharacter* Enemy)
{

	ULifeComponent* MyLifeComponent = Me->GetComponentByClass<ULifeComponent>();
	ULifeComponent* EnemyLifeComponent = Enemy->GetComponentByClass<ULifeComponent>();
	
	if (MyLifeComponent && EnemyLifeComponent)
	{
		SetHealth(Me->GetIsLeft(), MyLifeComponent->GetHp(), MyLifeComponent->GetHpMax());
		SetHealth(Enemy->GetIsLeft(), EnemyLifeComponent->GetHp(), EnemyLifeComponent->GetHpMax());
		
		// MyLifeComponent->OnHealthChanged.AddDynamic(this, &UAssslashHUD::OnMyHealthChanged);
		// EnemyLifeComponent->OnHealthChanged.AddDynamic(this, &UAssslashHUD::OnEnemyHealthChanged);
		// UE_LOG(LogAssslash, Warning, TEXT("[AssslashHUD] Delegate subscription succeeded!"));
	}
}



void UAssslashHUD::SetHealth(bool isLeft, float Health, float MaxHealth)
{
	if (isLeft)
	{
		SetHealthLeft(Health, MaxHealth);
	} else
	{
		SetHealthRight(Health, MaxHealth);
	}
}

void UAssslashHUD::SetHealthLeft(float Health, float MaxHealth) const
{
	if (LtHealthBar)
	{
		LtHealthBar->SetPercent(Health / MaxHealth);
		Text_Hp_Lt->SetText(FText::AsNumber(Health));
	}
}

void UAssslashHUD::SetHealthRight(float Health, float MaxHealth) const
{
	if (RtHealthBar)
	{
		RtHealthBar->SetPercent(Health / MaxHealth);
		Text_Hp_Rt->SetText(FText::AsNumber(Health));
	}
}

void UAssslashHUD::OnMyHealthChanged(float Health, float MaxHealth, bool IsLeft)
{
	UE_LOG(LogAssslash, Warning, TEXT("[AssslashHUD] OnMyHealthChanged : %f"), Health);
	SetHealth(IsLeft, Health, MaxHealth);
}

void UAssslashHUD::OnEnemyHealthChanged(float Health, float MaxHealth, bool IsLeft)
{
	UE_LOG(LogAssslash, Warning, TEXT("[AssslashHUD] OnEnemyHealthChanged : %f"), Health)
	SetHealth(IsLeft, Health, MaxHealth);
}

float UAssslashHUD::GetHealthRight()
{
	return HealthRight;
}

float UAssslashHUD::GetHealthLeft()
{
	return HealthLeft;
}


