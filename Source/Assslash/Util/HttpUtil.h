// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/Object.h"
#include "HttpUtil.generated.h"

/**
 * @class UHttpUtil
 * @brief Utility class to handle HTTP requests in Unreal Engine projects.
 *
 * UHttpUtil provides static methods to create and configure HTTP requests.
 * It simplifies the process of setting up HTTP requests and managing request headers.
 */

class UHTTPConfig;

UCLASS()
class ASSSLASH_API UHttpUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UHttpUtil();
	
	static TSharedRef<IHttpRequest, ESPMode::ThreadSafe> CreateRequest();

	static void SetupRequestHeaders(
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request,
		const FString& Endpoint,
		const FString& Verb
	);

	static FString JsonToString(const TSharedPtr<FJsonObject>& Json);
};
