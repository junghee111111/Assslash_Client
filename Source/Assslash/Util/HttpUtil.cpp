﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpUtil.h"

#include "HttpModule.h"
#include "Assslash/Assslash.h"
#include "Assslash/Config/HTTPConfig.h"

UHttpUtil::UHttpUtil()
{
	
}


TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UHttpUtil::CreateRequest()
{
	return FHttpModule::Get().CreateRequest();
}

void UHttpUtil::SetupRequestHeaders(
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request,
	const FString& Endpoint,
	const FString& Verb = TEXT("GET")
)
{
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("User-Agent"), TEXT("UnrealEngine/Assslash"));
	Request->SetHeader(TEXT("Accept"), TEXT("*/*"));
	
	static const UHTTPConfig* HttpConfig = GetDefault<UHTTPConfig>();

	if (HttpConfig)
	{
		FString FullUrl = HttpConfig->ApiBaseUrl + Endpoint;
		Request->SetURL(FullUrl);
		Request->SetVerb(Verb);

		UE_LOG(LogAssslash, Log, TEXT("[HttpUtil] [%s] %s \n\t...=> %s \n\t...=> %s"),
		*Verb, *FullUrl,
		*Request->GetHeader(TEXT("User-Agent")), *Request->GetHeader(TEXT("Content-Type"))
	)
	}

	
}

FString UHttpUtil::JsonToString(const TSharedPtr<FJsonObject>& Json)
{
	FString JsonString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
	return JsonString;
}
