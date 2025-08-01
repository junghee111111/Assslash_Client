// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetConfirm.h"

#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

void UWidgetConfirm::Show(const FText& TitleMessage, const FText& ContentMessage, bool bSimple)
{
	this->Title->SetText(TitleMessage);
	this->Content->SetText(ContentMessage);
	this->Simple = bSimple;

	if (bSimple == true)
	{
		Button_No->SetVisibility(ESlateVisibility::Collapsed);
	} else
	{
		Button_No->SetVisibility(ESlateVisibility::Visible);
	}
	
	this->PlayAnimation(init);
}
