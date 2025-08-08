// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetCommonTransition.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UWidgetCommonTransition : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* SlideIn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* SlideOut;

	UFUNCTION(BlueprintCallable)
	void Show();
	UFUNCTION(BlueprintCallable)
	void Hide();
};
