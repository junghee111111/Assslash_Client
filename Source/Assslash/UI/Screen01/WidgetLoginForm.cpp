// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetLoginForm.h"

#include "Assslash/Assslash.h"
#include "Assslash/Common/AssslashGameInstance.h"
#include "Assslash/Util/HttpUtil.h"
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
	ShowLoading(false);
	if (HttpResponse.IsValid())
	{
		if (HttpResponse->GetResponseCode() == 200)
		{
			UE_LOG(LogAssslash, Log, TEXT("Login Success"));
		} else
		{
			GetGameInstance<UAssslashGameInstance>()->ShowToastMessage(
				TEXT("Login Failed!")
				);
		}
	}
	else
	{
		GetGameInstance<UAssslashGameInstance>()->ShowToastMessage(TEXT("Can't connect to server.."));
	}
}

void UWidgetLoginForm::OnSubmitButtonClicked()
{
	ShowLoading(true);
	
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

		TSharedPtr<FJsonObject> JsonBody = MakeShareable(new FJsonObject);
		JsonBody->SetStringField(TEXT("username"), Username);
		JsonBody->SetStringField(TEXT("password"), Password);

		FString JsonBodyString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonBodyString);
		FJsonSerializer::Serialize(JsonBody.ToSharedRef(), Writer);

		Request->SetContentAsString(JsonBodyString);
		Request->ProcessRequest();
	}
}

void UWidgetLoginForm::ShowLoading(bool bIsLoading)
{
}
