// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "InfoIndicator.generated.h"

UCLASS()
class ASSSLASH_API AInfoIndicator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInfoIndicator();

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* IndicatorWidgetComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInfoIndicatorWidget> IndicatorWidgetClass;

	UPROPERTY()
	class UInfoIndicatorWidget* IndicatorWidget;
	
	


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
