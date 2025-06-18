#include "C:\Users\user\Documents\Unreal Projects\Assslash\Intermediate\Build\Win64\x64\AssslashEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h"
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
