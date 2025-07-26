// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashGameInstance.h"

void UAssslashGameInstance::SetAuthToken(const FString& Token, float ExpirationTimeInSeconds)
{
	AuthToken = Token;
}
