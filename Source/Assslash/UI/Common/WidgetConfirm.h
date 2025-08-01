// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetConfirm.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UWidgetConfirm : public UUserWidget
{
	GENERATED_BODY()

public:
	// ========== Animations ==========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* init;
	
	// ========== Widgets ==========
	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UTextBlock* Title;

	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class URichTextBlock* Content;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Simple;

	UFUNCTION(BlueprintCallable)
	void Show(const FText& TitleMessage, const FText& ContentMessage, bool bSimple);
};
