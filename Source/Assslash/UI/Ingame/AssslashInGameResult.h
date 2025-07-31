// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AssslashInGameResult.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ASSSLASH_API UAssslashInGameResult : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void PlayAnimShow();

	UFUNCTION()
	void PlayAnimKO();

	UFUNCTION()
	void PlayAnimWin();

	UFUNCTION()
	void PlayAnimResult();
	
protected:
	// ========== Animations ==========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* show;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* KO;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Win;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* Result;
};
