#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AssslashHUD.generated.h"

UCLASS(Abstract)
class ASSSLASH_API UAssslashHUD : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetHealthLeft(float Health, float MaxHealth);

	void SetHealthRight(float Health, float MaxHealth);

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* LtHealthBar;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* RtHealthBar;
	
	
};
