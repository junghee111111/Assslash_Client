// Fill out your copyright notice in the Description page of Project Settings.


#include "StringTableUtil.h"

FText UStringTableUtil::GetUIString(const FName& Key)
{
	FText Found = FText::FromStringTable(FName("ST_UI"), Key.ToString());
	return Found;
}
