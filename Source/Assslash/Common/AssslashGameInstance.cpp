// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashGameInstance.h"

#include "Animation/WidgetAnimation.h"
#include "Assslash/Assslash.h"
#include "Assslash/UI/Common/WidgetConfirm.h"
#include "Assslash/UI/Common/WidgetToastMessage.h"
#include "Blueprint/UserWidget.h"

void UAssslashGameInstance::Init()
{
	Super::Init();
	
}

void UAssslashGameInstance::SetAuthToken(const FString& Token, float ExpirationTimeInSeconds)
{
	AuthToken = Token;
}

void UAssslashGameInstance::ShowLoading(bool Show)
{
	if (Show==true)
	{
		if (!UI_Loading)
		{
			APlayerController* PC = GetFirstLocalPlayerController();
			UI_Loading = CreateWidget<UUserWidget>(PC,UI_LoadingClass);
			if (UI_Loading) UI_Loading->AddToViewport();
		}
		UI_Loading->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		if (UI_Loading) UI_Loading->SetVisibility(ESlateVisibility::Collapsed);
	}
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

void UAssslashGameInstance::ShowConfirm(const FText Title, const FText Content, const bool bSimple)
{
	if (!UI_Confirm)
	{
		APlayerController* PC = GetFirstLocalPlayerController();
		UI_Confirm = CreateWidget<UWidgetConfirm>(PC,UI_ConfirmClass);
		
	}
	
	if (UI_Confirm)
	{
		UI_Confirm->AddToViewport();
		UI_Confirm->Show(Title, Content, bSimple);
	}
}

void UAssslashGameInstance::OpenLevel(const FName& LevelName, bool bAbsolute)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	check(PC);
	PC->ClientTravel(LevelName.ToString(), TRAVEL_Absolute,false);
}
