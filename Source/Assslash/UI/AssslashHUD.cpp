#include "AssslashHUD.h"

#include "Components/ProgressBar.h"


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


