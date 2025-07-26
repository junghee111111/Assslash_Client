// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AssslashGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UAssslashGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	FString AuthToken;
	FDateTime AuthTokenExpireTime;

public:
	UFUNCTION(BlueprintCallable, Category="Assslash Authentication")
	void SetAuthToken(const FString& Token, float ExpirationTimeInSeconds = 0.0f);
};
