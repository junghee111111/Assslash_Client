// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Blueprint/UserWidget.h"
#include "WidgetLoginForm.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UWidgetLoginForm : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ========== Widgets ==========
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* TextBox_Username;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* TextBox_Password;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* Button_Submit;
	
	UFUNCTION()
	void OnSubmitButtonClicked();

	UFUNCTION(BlueprintCallable)
	void ShowLoading(bool bIsLoading);

private:
	FHttpModule* Http;
};
