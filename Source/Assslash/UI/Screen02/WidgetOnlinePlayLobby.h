// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetOnlinePlayLobby.generated.h"

/**
 * 
 */
UCLASS()
class ASSSLASH_API UWidgetOnlinePlayLobby : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	void OnButtonBackClicked();
	void OnButtonMatchRandomClicked();
	void OnButtonMatchInviteClicked();

	bool bIsReject = false;

public:
	UFUNCTION(BlueprintCallable)
	void FetchUserInfo();
	
	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UButton* Button_Back;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UButton* Button_MatchRandom;

	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UButton* Button_MatchInvite;

	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UTextBlock* TextBlock_Name;

	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UTextBlock* TextBlock_SeasonName;

	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UTextBlock* TextBlock_WinRate;

	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UTextBlock* TextBlock_TotalWin;

	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UTextBlock* TextBlock_TotalLose;

	UPROPERTY(EditAnywhere, meta=(BindWidget), BlueprintReadWrite)
	class UCheckBox* CheckBox_Reject;
};
