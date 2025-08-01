﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetRegisterForm.h"

#include "Assslash/Common/AssslashGameInstance.h"
#include "Assslash/Util/HttpUtil.h"
#include "Assslash/Util/StringTableUtil.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UWidgetRegisterForm::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_Submit)
	{
		Button_Submit->OnClicked.AddDynamic(this,
			&UWidgetRegisterForm::OnRegisterButtonClicked);
	}
}

void UWidgetRegisterForm::NativeDestruct()
{
	Super::NativeDestruct();
	if (Button_Submit)
	{
		Button_Submit->OnClicked.RemoveDynamic(
			this, &UWidgetRegisterForm::OnRegisterButtonClicked);
	}
}

void UWidgetRegisterForm::OnRegisterResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	bool bWasSuccessful)
{
	GetGameInstance<UAssslashGameInstance>()->ShowLoading(false);
}

void UWidgetRegisterForm::OnRegisterButtonClicked()
{
	FString Username = TEXT("");
	FString Password = TEXT("");
	FString PasswordCheck = TEXT("");
	FString Name = TEXT("");

	if (!TextBox_Name || !TextBox_Username || !TextBox_Password || !TextBox_Password_Check)
	{
		return;
	}

	Username = TextBox_Username->GetText().ToString().TrimStartAndEnd();
	Password = TextBox_Password->GetText().ToString().TrimStartAndEnd();
	PasswordCheck = TextBox_Password_Check->GetText().ToString().TrimStartAndEnd();
	Name = TextBox_Name->GetText().ToString().TrimStartAndEnd();

	if (Username.IsEmpty() || Password.IsEmpty() || PasswordCheck.IsEmpty() || Name.IsEmpty())
	{
		GetGameInstance<UAssslashGameInstance>()->ShowConfirm(
			UStringTableUtil::GetUIString(TEXT("UI_CONFIRM_DEFAULT_TITLE")),
			UStringTableUtil::GetUIString(TEXT("FORM_EMPTY_NOT_ALLOWED")),
			true
		);
		return;
	}
	
	if (Password!=PasswordCheck)
	{
		GetGameInstance<UAssslashGameInstance>()->ShowConfirm(
			UStringTableUtil::GetUIString(TEXT("UI_CONFIRM_DEFAULT_TITLE")),
			UStringTableUtil::GetUIString(TEXT("REGISTER_PASSWORD_CHECK_FAILED")),
			true
		);
		return;
	}

	GetGameInstance<UAssslashGameInstance>()->ShowLoading(true);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = UHttpUtil::CreateRequest();
	UHttpUtil::SetupRequestHeaders(Request, TEXT("register"), TEXT("POST"));
	Request->OnProcessRequestComplete().BindUObject(this, &UWidgetRegisterForm::OnRegisterResponse);

	TSharedPtr<FJsonObject> Body = MakeShareable(new FJsonObject);
	Body->SetStringField(TEXT("username"), Username);
	Body->SetStringField(TEXT("password"), Password);
	Body->SetStringField(TEXT("name"), Name);

	Request->SetContentAsString(UHttpUtil::JsonToString(Body));
	Request->ProcessRequest();
}
