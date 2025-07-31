// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InfoIndicatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UInfoIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UTextBlock* Info;
};
