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

public:

	UFUNCTION(BlueprintCallable, Category="Authentication")
	FString GetAuthToken();

	UFUNCTION(BlueprintCallable, Category="Authentication")
	void SetAuthToken(const FString& Token);

private:
	FDateTime AuthTokenExpireTime;
	
	UPROPERTY()
	class UWidgetToastMessage* UI_ToastMessage = nullptr;
	UPROPERTY()
	class UUserWidget* UI_Loading = nullptr;
	UPROPERTY()
	class UUserWidget* UI_Transition = nullptr;
	UPROPERTY()
	class UWidgetConfirm* UI_Confirm = nullptr;

	class UAudioComponent* BGMComponent = nullptr;
	float BGMVolume = 1.0f;
	float BGMLowPassFilterHz = 1000.0f;

	UPROPERTY()
	USoundBase* BGMCurrentPath;

public:
	virtual void Init() override;
	virtual void OnStart() override;
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
	
	// ========== Widgets =========
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Common UI")
	TSubclassOf<class UWidgetToastMessage> UI_ToastMessageClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Common UI")
	TSubclassOf<class UUserWidget> UI_LoadingClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Common UI")
	TSubclassOf<class UWidgetConfirm> UI_ConfirmClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Common UI")
	TSubclassOf<class UUserWidget> UI_TransitionClass;
	
	UFUNCTION(BlueprintCallable, Category="Common UI")
	void ShowLoading(bool Show);

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowToastMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowConfirm(FText Title, FText Content, bool bSimple);

    UFUNCTION(BlueprintCallable, Category="Level")
    void OpenLevel(const FName& LevelName, bool bAbsolute = false);

	UFUNCTION(BlueprintCallable, Category="Audio")
	void PlayBGM(USoundBase* NewBGM);
};
