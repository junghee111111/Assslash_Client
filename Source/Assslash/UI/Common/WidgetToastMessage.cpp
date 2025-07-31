// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetToastMessage.h"

#include "Components/RichTextBlock.h"

void UWidgetToastMessage::ShowToastMessage(FString Message)
{
	PlayAnimation(show,0,1);
	Text_ToastMessage->SetText(FText::FromString(Message));
}
