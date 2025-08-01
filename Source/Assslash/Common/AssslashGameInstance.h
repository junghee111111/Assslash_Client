// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AssslashGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UAssslashGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	FString AuthToken;
	FDateTime AuthTokenExpireTime;
	
	UPROPERTY()
	class UWidgetToastMessage* UI_ToastMessage = nullptr;

	UPROPERTY()
	class UUserWidget* UI_Loading = nullptr;

public:
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category="Assslash Authentication")
	void SetAuthToken(const FString& Token, float ExpirationTimeInSeconds = 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Common UI")
	TSubclassOf<class UWidgetToastMessage> UI_ToastMessageClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Common UI")
	TSubclassOf<class UUserWidget> UI_LoadingClass;
	
	UFUNCTION(BlueprintCallable, Category="Common UI")
	void ShowLoading(bool Show);

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowToastMessage(const FString& Message);
};
