// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StringTableUtil.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UStringTableUtil : public UObject
{
	GENERATED_BODY()

public:
	static FText GetUIString(const FTextKey Key);

	static UStringTable* UIStringTable;
};
