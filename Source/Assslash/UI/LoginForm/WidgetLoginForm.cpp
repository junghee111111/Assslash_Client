// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetLoginForm.h"

#include "Assslash/Assslash.h"
#include "Components/Button.h"

void UWidgetLoginForm::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UE_LOG(LogAssslash, Log, TEXT("[LOGINFORM] NativeOnInitialized"));

	Http = &FHttpModule::Get();
}

void UWidgetLoginForm::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Submit)
	{
		Button_Submit->OnClicked.AddDynamic(
			this, &UWidgetLoginForm::OnSubmitButtonClicked);
	}
}

void UWidgetLoginForm::NativeDestruct()
{
	Super::NativeDestruct();
	if (Button_Submit)
	{
		Button_Submit->OnClicked.RemoveDynamic(
			this, &UWidgetLoginForm::OnSubmitButtonClicked);
	}
}

void UWidgetLoginForm::OnSubmitButtonClicked()
{
	ShowLoading(true);

	SendHTTPPost("");
}

void UWidgetLoginForm::ShowLoading(bool bIsLoading)
{
}
