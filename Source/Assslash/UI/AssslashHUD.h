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
	void SetHealth(bool isLeft, float Health, float MaxHealth);

	void SetHealthLeft(float Health, float MaxHealth) const;

	void SetHealthRight(float Health, float MaxHealth) const;

	UFUNCTION()
	void OnMyHealthChanged(float Health, float MaxHealth, bool IsLeft);

	UFUNCTION()
	void OnEnemyHealthChanged(float Health, float MaxHealth, bool IsLeft);
	
	UFUNCTION(BlueprintCallable)
	float GetHealthLeft();

	UFUNCTION(BlueprintCallable)
	float GetHealthRight();

	// ========== Widgets ==========
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* LtHealthBar;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* RtHealthBar;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* Text_Hp_Lt;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* Text_Hp_Rt;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* Text_Name_Lt;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* Text_Name_Rt;
	

private:
	float HealthLeft;
	float HealthRight;
	
};
