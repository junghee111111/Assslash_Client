// Fill out your copyright notice in the Description page of Project Settings.


#include "StringTableUtil.h"

#include "Assslash/Assslash.h"
#include "Internationalization/StringTable.h"
#include "Internationalization/StringTableCore.h"

UStringTable* UStringTableUtil::UIStringTable = nullptr;

FText UStringTableUtil::GetUIString(const FTextKey Key)
{
	if (!UIStringTable)
	{
		FSoftObjectPath SoftPath(TEXT("/Game/Base/Strings/ST_UI"));
		UIStringTable = Cast<UStringTable>(SoftPath.TryLoad());
	}
	if (UIStringTable)
	{
		FString Found = UIStringTable->GetStringTable()->FindEntry(Key)->GetSourceString();
		UE_LOG(LogAssslash, Warning, TEXT("GetUIString [%s] : %s"), *Key.ToString(), *Found);
		return FText::FromString(Found);
	} 
	return FText::FromString(TEXT("STRING TABLE NOT FOUND"));
}
