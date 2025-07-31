// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetToastMessage.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UWidgetToastMessage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* show;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class URichTextBlock* Text_ToastMessage;

	UFUNCTION(BlueprintCallable)
	void ShowToastMessage(FString Message);
};
