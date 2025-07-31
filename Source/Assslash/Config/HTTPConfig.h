// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "HTTPConfig.generated.h"

/**
 * 
 */
UCLASS(config=Game, defaultconfig)
class ASSSLASH_API UHTTPConfig : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UHTTPConfig();

	virtual FName GetCategoryName() const override;

	UPROPERTY(Config, EditAnywhere, Category = "HTTP", meta = (DisplayName = "API Base URL"))
	FString ApiBaseUrl;

	/** API 요청 타임아웃 (초) */
	UPROPERTY(config, EditAnywhere, Category="Server", meta=(DisplayName="Request Timeout", ClampMin="1.0"))
	float RequestTimeoutSeconds;

	/** 개발 모드 활성화 여부 */
	UPROPERTY(config, EditAnywhere, Category="Debug", meta=(DisplayName="Development Mode"))
	bool bDevelopmentMode;

};
