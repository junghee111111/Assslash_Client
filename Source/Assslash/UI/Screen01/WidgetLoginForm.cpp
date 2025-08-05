// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetLoginForm.h"

#include "Assslash/Assslash.h"
#include "Assslash/Common/AssslashGameInstance.h"
#include "Assslash/Util/HttpUtil.h"
#include "Assslash/Util/StringTableUtil.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Interfaces/IHttpResponse.h"

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

void UWidgetLoginForm::OnLoginResponse(
	FHttpRequestPtr HttpRequest,
	FHttpResponsePtr HttpResponse,
	bool bWasSuccessful
	)
{
	UAssslashGameInstance* GI = GetGameInstance<UAssslashGameInstance>();
	check(GI);
	GI->ShowLoading(false);
	
	if (!HttpResponse.IsValid())
	{
		GI->ShowToastMessage(
				UStringTableUtil::GetUIString(TEXT("LOGIN_UNEXPECTED_ERROR")).ToString()
			);
		return;
	}
	
	if (HttpResponse->GetResponseCode() == 200)
	{
		FString RawToken = HttpResponse->GetHeader(TEXT("authorization"));
		if (RawToken.IsEmpty())
		{
			GI->ShowToastMessage(
				UStringTableUtil::GetUIString(TEXT("LOGIN_NO_TOKEN")).ToString()
			);
			return;
		}
		FString Token = RawToken.Replace(TEXT("Bearer "), TEXT(""));
		GI->SetAuthToken(Token);
		GI->ShowLoading(true);
		GI->OpenLevel("/Game/OnlineLobby");
	} else if (HttpResponse->GetResponseCode() == 401)
	{
		GI->ShowToastMessage(
			UStringTableUtil::GetUIString(TEXT("LOGIN_401")).ToString()
		);
	} else
	{
		GI->ShowToastMessage(
			UStringTableUtil::GetUIString(TEXT("LOGIN_UNEXPECTED_ERROR")).ToString()
		);
	}
}

void UWidgetLoginForm::OnSubmitButtonClicked()
{
	FString Username = TEXT("");
	FString Password = TEXT("");
	if (TextBox_Username && TextBox_Password)
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
		UHttpUtil::CreateRequest();
		UHttpUtil::SetupRequestHeaders(Request, TEXT("login"), TEXT("POST"));
		Request->OnProcessRequestComplete().BindUObject(this, &UWidgetLoginForm::OnLoginResponse);
		
		Username = TextBox_Username->GetText().ToString();
		Password = TextBox_Password->GetText().ToString();

		if (Username.IsEmpty() || Password.IsEmpty())
		{
			GetGameInstance<UAssslashGameInstance>()->ShowConfirm(
				UStringTableUtil::GetUIString(TEXT("UI_CONFIRM_DEFAULT_TITLE")),
				UStringTableUtil::GetUIString(TEXT("FORM_EMPTY_NOT_ALLOWED")),
				true
			);
			return;
		}

		
		TSharedPtr<FJsonObject> JsonBody = MakeShareable(new FJsonObject);
		JsonBody->SetStringField(TEXT("username"), Username);
		JsonBody->SetStringField(TEXT("password"), Password);

		GetGameInstance<UAssslashGameInstance>()->ShowLoading(true);
		Request->SetContentAsString(UHttpUtil::JsonToString(JsonBody));
		Request->ProcessRequest();
	}
}
