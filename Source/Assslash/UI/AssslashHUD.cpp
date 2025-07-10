#include "AssslashHUD.h"

#include "Assslash/Assslash.h"
#include "Assslash/Character/AssslashCharacter.h"
#include "Assslash/Character/Interface/LifeComponent.h"
#include "Components/ProgressBar.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"


void UAssslashHUD::NativeConstruct()
{
	Super::NativeConstruct();
	AGameStateBase* GameState = GetWorld()->GetGameState();
	TArray<APlayerState*> PlayerStates = GameState->PlayerArray;
	UE_LOG(LogAssslash, Warning, TEXT("Assslash HUD : PlayerStates Num : %d"), PlayerStates.Num());
	for (int i = 0; i < PlayerStates.Num(); i++)
	{
		APlayerState* pState = PlayerStates[i];
		float Hp = 0;
		float HpMax = 0;
		APawn* pPawn = pState->GetPawn();
		if (pPawn)
		{
			// cast to AssslashCharacter
			AAssslashCharacter* pCharacter = Cast<AAssslashCharacter>(pPawn);
			if (pCharacter)
			{
				// access to LifeComponent
				ULifeComponent* life = pCharacter->GetComponentByClass<ULifeComponent>();
				Hp = life->GetHp();
				HpMax = life->GetHpMax();
			}
		}
		if (i==0)
		{
			SetHealthLeft(Hp, HpMax);
			UE_LOG(LogAssslash, Warning, TEXT("Left Hp : %f %f"), Hp, HpMax);
		} else
		{
			SetHealthRight(Hp, HpMax);
			UE_LOG(LogAssslash, Warning, TEXT("Right Hp : %f %f"), Hp, HpMax);
		}
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


