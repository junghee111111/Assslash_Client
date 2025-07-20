// Fill out your copyright notice in the Description page of Project Settings.


#include "AssslashGameMode.h"

#include "EngineUtils.h"
#include "ToolBuilderUtil.h"
#include "Assslash/Assslash.h"
#include "Assslash/Character/AssslashCharacter.h"
#include "Assslash/Character/AssslashPlayerController.h"
#include "Components/AudioComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


AAssslashGameMode::AAssslashGameMode()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> BgmAsset(TEXT("'/Game/Data/Sound/BGM/InGame.InGame'"));
	if (BgmAsset.Succeeded())
	{
		BGMCue = BgmAsset.Object;
	}
	
	PlayerControllerClass = AAssslashPlayerController::StaticClass();
	DefaultPawnClass = AAssslashCharacter::StaticClass();

	// Audio Component Init.
	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	BGMComponent->SetupAttachment(RootComponent);
	BGMComponent->bAutoActivate = true;
		
}



void AAssslashGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogAssslash, Log, TEXT("Game is running : %s %s"), *MapName, *Options)

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		FreePlayerStarts.Add(*It);
		UE_LOG(LogAssslash, Log, TEXT("Found player start : %s"), *(*It)->GetName())
	}
}

void AAssslashGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	if (FreePlayerStarts.Num() == 0)
	{
		ErrorMessage = TEXT("No free player starts");
	}
	
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

FString AAssslashGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
                                         const FString& Options, const FString& Portal)
{
	if (FreePlayerStarts.Num() == 0)
	{
		UE_LOG(LogAssslash, Error, TEXT("No free player starts @ InitNewPlayer"));
		return FString(TEXT("No free player starts"));
	}

	NewPlayerController->StartSpot = FreePlayerStarts.Pop();
	UE_LOG(LogAssslash, Log, TEXT("New player '%s' start : %s"),*NewPlayerController->GetName(), *NewPlayerController->StartSpot->GetName())

	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	if (Result.IsEmpty())
	{
		// OnPlayerConnected Delegate 호출한다.
		OnPlayerConnected.Broadcast(NewPlayerController);
	}
	
	return Result;
}

void AAssslashGameMode::HandleAllPlayersReady()
{
	AAssslashCharacter* Player1Char = Cast<AAssslashCharacter>(ReadyPlayers[0]->GetPawn());
	AAssslashCharacter* Player2Char = Cast<AAssslashCharacter>(ReadyPlayers[1]->GetPawn());
	
	

	if (IsValid(Player1Char) && IsValid(Player2Char))
	{
		// 서로를 Enemy로 설정 (서버에서만 실행됨)
		Player1Char->SetEnemy(Player2Char); 
		Player2Char->SetEnemy(Player1Char);
		Player2Char->Server_SetInitialRotation();
		PlayLevelBGM();
		UE_LOG(LogAssslash, Log, TEXT("[GAMEMODE] Set enemies. %s is enemy of %s, and vice versa."), *Player1Char->GetName(), *Player2Char->GetName());
	}
}

void AAssslashGameMode::PostLogin(APlayerController* NewPlayerController)
{
	Super::PostLogin(NewPlayerController);
	UE_LOG(LogAssslash, Log, TEXT("[GAMEMODE] PostLogin : %s"), *NewPlayerController->GetName());
	
	APlayerController* PC = Cast<APlayerController>(NewPlayerController);
	if (NewPlayerController && NewPlayerController->GetPawn())
	{
		if (!ReadyPlayers.Contains(NewPlayerController))
		{
			ReadyPlayers.Add(PC);
			OnPlayerPawnReady.Broadcast(PC);
			if (ReadyPlayers.Num() == 2)
			{
				HandleAllPlayersReady();
			}
		}
	}
}

void AAssslashGameMode::RestartPlayer(AController* NewPlayerController)
{
	Super::RestartPlayer(NewPlayerController);
	UE_LOG(LogAssslash, Log, TEXT("[GAMEMODE] RestartPlayer : %s"), *NewPlayerController->GetName())
	
}


void AAssslashGameMode::StartPlay()
{
	Super::StartPlay();
    
	// 게임 시작 시 BGM 재생
}

void AAssslashGameMode::PlayLevelBGM()
{
	// 현재 레벨 이름 가져오기
	FString LvName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	FString BGMPath = BGMBasePath + "InGame";

	USoundBase* BGMSound = LoadObject<USoundBase>(nullptr, *BGMPath);

	if (BGMSound)
	{
		// 서버에서 멀티캐스트 함수 호출
		Multicast_PlayBGM(BGMSound, BGMVolume);
		UE_LOG(LogAssslash, Display, TEXT("[GAMEMODE] play BGM : %s"), *BGMPath);
	}
	else
	{
		UE_LOG(LogAssslash, Error, TEXT("[GAMEMODE] BGM not found : %s"), *BGMPath);
	}
}

void AAssslashGameMode::Multicast_PlayBGM_Implementation(USoundBase* BGMSound, float Volume)
{
	// 클라이언트에서 사운드 재생
	if (BGMSound)
	{
		UE_LOG(LogAssslash, Display, TEXT("[GAMEMODE] play BGM at client : %s"), *GetName());
		UGameplayStatics::PlaySound2D(
			GetWorld(),
			BGMSound,
			Volume,
			1.0f,  // 피치
			0.0f,  // 시작 시간
			nullptr,  // 사운드 동시 재생 설정
			nullptr,  // 소유자
			true    // 반복 재생
		);
	}
}
