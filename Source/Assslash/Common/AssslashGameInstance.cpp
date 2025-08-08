// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashGameInstance.h"

#include "Animation/WidgetAnimation.h"
#include "Assslash/Assslash.h"
#include "Assslash/UI/Common/WidgetCommonTransition.h"
#include "Assslash/UI/Common/WidgetConfirm.h"
#include "Assslash/UI/Common/WidgetToastMessage.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

FString UAssslashGameInstance::GetAuthToken()
{
	return AuthToken;
}

void UAssslashGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogAssslash, Log, TEXT("[GAMEINSTANCE] Init..!"));
}

void UAssslashGameInstance::OnStart()
{
	Super::OnStart();
	UE_LOG(LogAssslash, Log, TEXT("[GAMEINSTANCE] OnStart"))
}

void UAssslashGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
	UE_LOG(LogAssslash, Log, TEXT("[GAMEINSTANCE] OnWorldChanged"));
}

void UAssslashGameInstance::SetAuthToken(const FString& Token)
{
	AuthToken = Token;
	return;
}

void UAssslashGameInstance::ShowLoading(bool Show)
{
	if (Show==true)
	{
		if (!UI_Loading)
		{
			APlayerController* PC = GetFirstLocalPlayerController();
			UI_Loading = CreateWidget<UUserWidget>(PC,UI_LoadingClass);
			if (UI_Loading) UI_Loading->AddToViewport();
		}
		UI_Loading->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		if (UI_Loading) UI_Loading->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAssslashGameInstance::ShowToastMessage(const FString& Message)
{
	
	if (!UI_ToastMessage)
	{
		APlayerController* PC = GetFirstLocalPlayerController();
		UI_ToastMessage = CreateWidget<UWidgetToastMessage>(PC,UI_ToastMessageClass);
		if (UI_ToastMessage) UI_ToastMessage->AddToViewport();
	}
	UI_ToastMessage->ShowToastMessage(Message);
}

void UAssslashGameInstance::ShowConfirm(const FText Title, const FText Content, const bool bSimple)
{
	if (!UI_Confirm)
	{
		APlayerController* PC = GetFirstLocalPlayerController();
		UI_Confirm = CreateWidget<UWidgetConfirm>(PC,UI_ConfirmClass);
		
	}
	
	if (UI_Confirm)
	{
		UI_Confirm->AddToViewport();
		UI_Confirm->Show(Title, Content, bSimple);
	}
}

void UAssslashGameInstance::OpenLevel(const FName& LevelName, bool bAbsolute)
{
	if (!UI_Transition || UI_Transition->IsValidLowLevel() == false)
	{
		UE_LOG(LogAssslash, Log, TEXT("[GAMEINSTANCE] OpenLevel :: Add UI_Transition"));
		APlayerController* PC = GetFirstLocalPlayerController();
		UI_Transition = CreateWidget<UWidgetCommonTransition>(PC,UI_TransitionClass);
		UI_Transition->AddToViewport();
	}
	if (UI_Transition && UI_Transition->IsValidLowLevel())
	{
		UE_LOG(LogAssslash, Log, TEXT("[GAMEINSTANCE] UI_Transition Added to viewport"));
		UI_Transition->Show();
	}
	if (BGMComponent->IsValidLowLevel())
	{
		if (BGMComponent->IsPlaying())
		{
			BGMComponent->FadeOut(0.3f, 0.0f, EAudioFaderCurve::Linear);
		}
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this, LevelName]()
		{
			this->TravelLevel(LevelName);
		},
		1.0f,
		false
	);
}

void UAssslashGameInstance::TravelLevel(const FName& LevelName)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	check(PC);
	PC->ClientTravel(
				LevelName.ToString(),
				TRAVEL_Absolute,
				true);
}

void UAssslashGameInstance::PlayBGM(USoundBase* NewBGM)
{
	// 입력 확인
	if (!NewBGM)
	{
		UE_LOG(LogAssslash, Error, TEXT("BGM is empty"));
		return;
	}

	// 월드 확인
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogAssslash, Error, TEXT("Invalid World context"));
		return;
	}

	// 동일한 BGM인지 확인
	if (BGMCurrentPath == NewBGM && BGMComponent && BGMComponent->IsPlaying())
	{
		return;
	}
    
	// 현재 BGM 저장
	BGMCurrentPath = NewBGM;

	// 기존 컴포넌트 정리
	if (BGMComponent && BGMComponent->IsValidLowLevel())
	{
		if (BGMComponent->IsPlaying())
		{
			BGMComponent->Stop();
		}
		BGMComponent = nullptr;
	}
    
	// 새 사운드 스폰
	BGMComponent = UGameplayStatics::SpawnSound2D(World, NewBGM, 1.0f, 1.0f, 0.0f, nullptr, true, true);
    
	if (BGMComponent)
	{
		BGMComponent->bAutoDestroy = false;
		BGMComponent->bIsUISound = true;
		UE_LOG(LogAssslash, Log, TEXT("Playing BGM: %s"), *NewBGM->GetName());
	}
	else
	{
		UE_LOG(LogAssslash, Error, TEXT("Failed to create BGM component"));
	}
}
