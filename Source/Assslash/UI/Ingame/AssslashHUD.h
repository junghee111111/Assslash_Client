#pragma once

#include "CoreMinimal.h"
#include "Assslash/Character/AssslashCharacter.h"
#include "Assslash/Character/Interface/LifeComponent.h"
#include "Blueprint/UserWidget.h"
#include "AssslashHUD.generated.h"

UCLASS(Abstract)
class ASSSLASH_API UAssslashHUD : public UUserWidget
{
public:
	void InitWithEnemy(AAssslashCharacter* Me, AAssslashCharacter* Enemy);

private:
	GENERATED_BODY()

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	// ========== Animations ==========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ShakeRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ShakeLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* ShowHitBG;
	
public:
	void SetHealth(bool isLeft, float Health, float MaxHealth);

	void SetHealthSmoothly(bool isLeft, float Health, float MaxHealth);

	void SetHealthLeft(float Health, float MaxHealth);

	void SetHealthRight(float Health, float MaxHealth);

	UFUNCTION(BlueprintCallable)
	void PlayShakeLeft();

	UFUNCTION(BlueprintCallable)
	void PlayShakeRight();
	void PlayerShowHitBG();

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
	UPROPERTY()
	float HealthLeft;

	UPROPERTY()
	float HealthRight;

	UPROPERTY()
	bool bIsImLeft = false;

	UPROPERTY()
	ULifeComponent* MyLifeComponent = nullptr;
	
	UPROPERTY()
	ULifeComponent* EnemyLifeComponent = nullptr;
	
};
