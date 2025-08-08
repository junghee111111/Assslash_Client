// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetOnlinePlayLobby.h"

#include "Assslash/Common/AssslashGameInstance.h"
#include "Components/Button.h"

void UWidgetOnlinePlayLobby::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Back) Button_Back->OnClicked.AddDynamic(
		this, &UWidgetOnlinePlayLobby::OnButtonBackClicked);
}

void UWidgetOnlinePlayLobby::OnButtonBackClicked()
{
	UAssslashGameInstance* GI = Cast<UAssslashGameInstance>(GetGameInstance());
	GI->OpenLevel(TEXT("/Game/MainMenu"));
}

void UWidgetOnlinePlayLobby::OnButtonMatchRandomClicked()
{
}

void UWidgetOnlinePlayLobby::OnButtonMatchInviteClicked()
{
}

void UWidgetOnlinePlayLobby::FetchUserInfo()
{
}
