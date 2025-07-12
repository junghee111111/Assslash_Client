#pragma once

#include "CoreMinimal.h"
#include "Assslash/Character/AssslashCharacter.h"
#include "Blueprint/UserWidget.h"
#include "AssslashHUD.generated.h"

UCLASS(Abstract)
class ASSSLASH_API UAssslashHUD : public UUserWidget
{
public:
	void InitWithEnemy(AAssslashCharacter* Me, AAssslashCharacter* Enemy);

private:
	GENERATED_BODY()

public:
	void SetHealth(FString PlayerIndex, float Health, float MaxHealth);

	void SetHealthLeft(float Health, float MaxHealth);

	void SetHealthRight(float Health, float MaxHealth);

	UFUNCTION(BlueprintCallable)
	float GetHealthLeft();

	UFUNCTION(BlueprintCallable)
	float GetHealthRight();

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* LtHealthBar;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* RtHealthBar;

private:
	float HealthLeft;
	float HealthRight;
	
};
