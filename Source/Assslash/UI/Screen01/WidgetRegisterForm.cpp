// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetRegisterForm.h"

#include "Assslash/Assslash.h"
#include "Assslash/Common/AssslashGameInstance.h"
#include "Assslash/Util/HttpUtil.h"
#include "Assslash/Util/StringTableUtil.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Interfaces/IHttpResponse.h"

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

void UWidgetRegisterForm::OnRegisterResponse(
	FHttpRequestPtr HttpRequest,
	FHttpResponsePtr HttpResponse,
	bool bWasSuccessful)
{
	GetGameInstance<UAssslashGameInstance>()->ShowLoading(false);
	UAssslashGameInstance* GI = GetGameInstance<UAssslashGameInstance>();

	UE_LOG(LogAssslash, Warning, TEXT("OnRegisterResponse : %s"), *HttpRequest->GetURL());
	UE_LOG(LogAssslash, Warning, TEXT("OnRegisterResponse : %s"), *HttpResponse->GetContentType());
	// UE_LOG(LogAssslash, Warning, TEXT("OnRegisterReq : %s"), HttpRequest->GetContent());
	UE_LOG(LogAssslash, Warning, TEXT("OnRegisterResponse : %s"), *HttpResponse->GetContentAsString());

	if (!HttpResponse.IsValid())
	{
		GI->ShowToastMessage(
			UStringTableUtil::GetUIString(TEXT("BACKEND_INVALID_RESPONSE")).ToString()
			);
		return;
	}

	if (HttpResponse->GetResponseCode() == 200)
	{
		GI->ShowConfirm(
			UStringTableUtil::GetUIString(TEXT("UI_CONFIRM_DEFAULT_TITLE")),
			UStringTableUtil::GetUIString(TEXT("REGISTER_DONE")),
			true
			);
	} else if (HttpResponse->GetResponseCode() == 401)
	{
		GI->ShowConfirm(
			UStringTableUtil::GetUIString(TEXT("UI_CONFIRM_DEFAULT_TITLE")),
			UStringTableUtil::GetUIString(TEXT("MODE_SINGLE")),
			true
			);
	} else if (HttpResponse->GetResponseCode() == 403)
	{
		//make body to parseable json
		TSharedPtr<FJsonObject> Resp = UHttpUtil::StringToJson(HttpResponse->GetContentAsString());
		FString message = Resp->GetStringField(TEXT("message"));
		if (message == TEXT("USERNAME_EXISTS"))
		{
			GI->ShowToastMessage(
				UStringTableUtil::GetUIString(TEXT("REGISTER_USERNAME_EXIST")).ToString()
			);
		} else if (message == TEXT("NAME_EXISTS"))
		{
			GI->ShowToastMessage(
				UStringTableUtil::GetUIString(TEXT("REGISTER_NAME_EXIST")).ToString()
			);
		}
		return;
	} else
	{
		GI->ShowToastMessage(
			UStringTableUtil::GetUIString(TEXT("BACKEND_INVALID_RESPONSE")).ToString()
		);
	}
}

void UWidgetRegisterForm::OnRegisterButtonClicked()
{
	FString Username = TEXT("");
	FString Password = TEXT("");
	FString PasswordCheck = TEXT("");
	FString Name = TEXT("");
	UAssslashGameInstance* GI = GetGameInstance<UAssslashGameInstance>();

	check(GI);

	if (!TextBox_Name || !TextBox_Username || !TextBox_Password || !TextBox_Password_Check)
	{
		return;
	}

	Username = TextBox_Username->GetText().ToString().TrimStartAndEnd();
	Password = TextBox_Password->GetText().ToString().TrimStartAndEnd();
	PasswordCheck = TextBox_Password_Check->GetText().ToString().TrimStartAndEnd();
	Name = TextBox_Name->GetText().ToString().TrimStartAndEnd();
	bool PolicyAccept = CheckBox_Agree->IsChecked();

	if (Username.IsEmpty() || Password.IsEmpty() || PasswordCheck.IsEmpty() || Name.IsEmpty())
	{
		GI->ShowConfirm(
			UStringTableUtil::GetUIString(TEXT("UI_CONFIRM_DEFAULT_TITLE")),
			UStringTableUtil::GetUIString(TEXT("FORM_EMPTY_NOT_ALLOWED")),
			true
		);
		return;
	}

	if (!PolicyAccept)
	{
		GI->ShowConfirm(
			UStringTableUtil::GetUIString(TEXT("UI_CONFIRM_DEFAULT_TITLE")),
			UStringTableUtil::GetUIString(TEXT("REGISTER_AGREE_NEED")),
			true
		);
		return;
	};
	
	if (Password!=PasswordCheck)
	{
		GI->ShowConfirm(
			UStringTableUtil::GetUIString(TEXT("UI_CONFIRM_DEFAULT_TITLE")),
			UStringTableUtil::GetUIString(TEXT("REGISTER_PASSWORD_CHECK_FAILED")),
			true
		);
		return;
	}

	GI->ShowLoading(true);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = UHttpUtil::CreateRequest();
	UHttpUtil::SetupRequestHeaders(Request, TEXT("auth/register"), TEXT("POST"));
	Request->OnProcessRequestComplete().BindUObject(this, &UWidgetRegisterForm::OnRegisterResponse);

	TSharedPtr<FJsonObject> Body = MakeShareable(new FJsonObject);
	Body->SetStringField(TEXT("username"), Username);
	Body->SetStringField(TEXT("password"), Password);
	Body->SetStringField(TEXT("name"), Name);

	Request->SetContentAsString(UHttpUtil::JsonToString(Body));
	Request->ProcessRequest();
}
