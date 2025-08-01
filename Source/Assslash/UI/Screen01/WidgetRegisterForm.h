// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/IHttpRequest.h"
#include "WidgetRegisterForm.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UWidgetRegisterForm : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	void OnRegisterResponse(FHttpRequestPtr HttpRequest,FHttpResponsePtr HttpResponse,bool bWasSuccessful);

	UFUNCTION()
	void OnRegisterButtonClicked();
	
public:
	
	// ========== Widgets ==========
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* Button_Back;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* TextBox_Username;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* TextBox_Password;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* TextBox_Password_Check;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UEditableTextBox* TextBox_Name;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UCheckBox* CheckBox_Agree;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* Button_Submit;
};
