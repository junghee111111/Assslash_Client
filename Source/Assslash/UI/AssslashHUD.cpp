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
	MyLifeComponent = Me->GetComponentByClass<ULifeComponent>();
	EnemyLifeComponent = Enemy->GetComponentByClass<ULifeComponent>();
	
	if (MyLifeComponent && EnemyLifeComponent)
	{
		SetHealth(Me->GetIsLeft(), MyLifeComponent->GetHp(), MyLifeComponent->GetHpMax());
		SetHealth(Enemy->GetIsLeft(), EnemyLifeComponent->GetHp(), EnemyLifeComponent->GetHpMax());

		if (Me->GetIsLeft()) bIsImLeft = true;
		// MyLifeComponent->OnHealthChanged.AddDynamic(this, &UAssslashHUD::OnMyHealthChanged);
		// EnemyLifeComponent->OnHealthChanged.AddDynamic(this, &UAssslashHUD::OnEnemyHealthChanged);
		// UE_LOG(LogAssslash, Warning, TEXT("[AssslashHUD] Delegate subscription succeeded!"));
	}
}

void UAssslashHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MyLifeComponent && EnemyLifeComponent)
	{
		// UE_LOG(LogAssslash, Warning, TEXT("[AssslashHUD] NativeTick %f / %f"), MyLifeComponent->GetHp(), EnemyLifeComponent->GetHp());

		SetHealthSmoothly(bIsImLeft, MyLifeComponent->GetHp(), MyLifeComponent->GetHpMax());
		SetHealthSmoothly(!bIsImLeft, EnemyLifeComponent->GetHp(), EnemyLifeComponent->GetHpMax());
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

void UAssslashHUD::SetHealthSmoothly(bool isLeft, float Health, float MaxHealth)
{
	if (isLeft)
	{
		float DisplayHealth = FMath::Lerp(HealthLeft, Health, 0.05f);
		SetHealthLeft(DisplayHealth, MaxHealth);
	} else
	{
		float DisplayHealth = FMath::Lerp(HealthRight, Health, 0.05f);
		SetHealthRight(DisplayHealth, MaxHealth);
	}
}

void UAssslashHUD::SetHealthLeft(float Health, float MaxHealth)
{
	if (LtHealthBar)
	{
		HealthLeft = Health;
		LtHealthBar->SetPercent(HealthLeft / MaxHealth);
		Text_Hp_Lt->SetText(FText::AsNumber(static_cast<uint32>(HealthLeft)));
	}
}

void UAssslashHUD::SetHealthRight(float Health, float MaxHealth)
{
	if (RtHealthBar)
	{
		HealthRight = Health;
		RtHealthBar->SetPercent(HealthRight / MaxHealth);
		Text_Hp_Rt->SetText(FText::AsNumber(static_cast<uint32>(HealthRight)));
	}
}

void UAssslashHUD::PlayShakeLeft()
{
	PlayAnimation(ShakeLeft);
}

void UAssslashHUD::PlayShakeRight()
{
	PlayAnimation(ShakeRight);
}

float UAssslashHUD::GetHealthRight()
{
	return HealthRight;
}

float UAssslashHUD::GetHealthLeft()
{
	return HealthLeft;
}


