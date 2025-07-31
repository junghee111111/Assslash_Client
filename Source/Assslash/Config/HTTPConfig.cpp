// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTPConfig.h"
UHTTPConfig::UHTTPConfig()
{
	// 기본값 설정
	ApiBaseUrl = TEXT("http://localhost:8080/");
	RequestTimeoutSeconds = 30.0f;
	bDevelopmentMode = false;
}

FName UHTTPConfig::GetCategoryName() const
{
	return FName("Game");
}
