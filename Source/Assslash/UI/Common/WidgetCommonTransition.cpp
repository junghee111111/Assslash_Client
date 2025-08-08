// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetCommonTransition.h"

void UWidgetCommonTransition::Show()
{
	this->SetVisibility(ESlateVisibility::Visible);
	this->PlayAnimation(SlideIn);
}

void UWidgetCommonTransition::Hide()
{
	this->PlayAnimation(SlideOut);
	// Collapse after 0.5 seconds
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
		},
		0.5f,
		false
		);
	
}
