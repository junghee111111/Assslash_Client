// Fill out your copyright notice in the Description page of Project Settings.


#include "InfoIndicator.h"

#include "InfoIndicatorWidget.h"


// Sets default values
AInfoIndicator::AInfoIndicator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 명시적으로 DefaultSceneRoot 생성
	USceneComponent* DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot; // RootComponent로 설정

	// WidgetComponent 생성 및 DefaultSceneRoot에 부착
	IndicatorWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	IndicatorWidgetComponent->SetupAttachment(DefaultSceneRoot); // DefaultSceneRoot에 부착
	
	AActor::SetLifeSpan(1.0f);
}

// Called when the game starts or when spawned
void AInfoIndicator::BeginPlay()
{
	Super::BeginPlay();

	
	// APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	// if (PlayerController && IndicatorWidgetClass)
	// {
	// 	UInfoIndicatorWidget* Widget = CreateWidget<UInfoIndicatorWidget>(
	// 		PlayerController,  // PlayerController를 소유자로 사용
	// 		IndicatorWidgetClass
	// 	);
 //        
	// 	if (Widget && IndicatorWidgetComponent)
	// 	{
	// 		IndicatorWidgetComponent->SetWidget(Widget);
 //            
	// 		// 3D 공간에 위젯이 보이도록 설정
	// 		IndicatorWidgetComponent->SetVisibility(true);
	// 	}
	// }

}

// Called every frame
void AInfoIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

