// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetOnlinePlayLobby.h"

#include "Assslash/Common/AssslashGameInstance.h"
#include "Assslash/Util/HttpUtil.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Interfaces/IHttpResponse.h"

void UWidgetOnlinePlayLobby::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Back) Button_Back->OnClicked.AddDynamic(
		this, &UWidgetOnlinePlayLobby::OnButtonBackClicked);

	FetchUserInfo();
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

void UWidgetOnlinePlayLobby::OnFetchUserInfoResponse(
	FHttpRequestPtr Request,
	FHttpResponsePtr Response,
	bool bWasSuccessful
	)
{
	UAssslashGameInstance* GI = Cast<UAssslashGameInstance>(GetGameInstance());

	GI->ShowLoading(false);

	if (Response->GetResponseCode() == 200)
	{
		TSharedPtr<FJsonObject> JsonObject = UHttpUtil::StringToJson(Response->GetContentAsString());
		
		TextBlock_Name->SetText(FText::FromString(
			JsonObject->GetObjectField("data")->GetStringField("name")));
		
		TextBlock_SeasonName->SetText(FText::FromString(
			JsonObject->GetObjectField("data")->GetStringField("seasonName")));
		TextBlock_WinRate->SetText(FText::FromString(
			JsonObject->GetObjectField("data")->GetStringField("seasonWinRate")));
		
		TextBlock_TotalWin->SetText(FText::FromString(
			JsonObject->GetObjectField("data")->GetStringField("seasonTotalWin")));
		TextBlock_TotalLose->SetText(FText::FromString(
			JsonObject->GetObjectField("data")->GetStringField("seasonTotalLose")));
	}
	else
	{
		GI->OpenLevel("/Game/MainMenu");
	}
}

void UWidgetOnlinePlayLobby::FetchUserInfo()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = UHttpUtil::CreateRequest();
	UAssslashGameInstance* GI = Cast<UAssslashGameInstance>(GetGameInstance());

	GI->ShowLoading(true);

	Request->SetHeader("Authorization", "Bearer "+GI->GetAuthToken());
	UHttpUtil::SetupRequestHeaders(
		Request, TEXT("auth/info"), TEXT("GET")
		);
	Request->OnProcessRequestComplete().BindUObject(this, &UWidgetOnlinePlayLobby::OnFetchUserInfoResponse);
	Request->ProcessRequest();
}
