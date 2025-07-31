// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashGameInstance.h"

#include "Animation/WidgetAnimation.h"
#include "Assslash/Assslash.h"
#include "Assslash/UI/ToastMessage/WidgetToastMessage.h"
#include "Blueprint/UserWidget.h"

void UAssslashGameInstance::Init()
{
	Super::Init();
	
}

void UAssslashGameInstance::SetAuthToken(const FString& Token, float ExpirationTimeInSeconds)
{
	AuthToken = Token;
}

void UAssslashGameInstance::ShowToastMessage(const FString& Message)
{
	
	if (!UI_ToastMessage)
	{
		APlayerController* PC = GetFirstLocalPlayerController();
		UI_ToastMessage = CreateWidget<UWidgetToastMessage>(PC,UI_ToastMessageClass);
		if (UI_ToastMessage) UI_ToastMessage->AddToViewport();
	}
	UI_ToastMessage->ShowToastMessage(Message);
}
