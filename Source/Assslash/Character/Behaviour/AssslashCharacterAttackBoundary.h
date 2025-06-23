// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssslashCharacterAttackBoundary.generated.h"

// 인자가 없는 동적 멀티캐스트 델리게이트 선언함. Dynamic은 Blueprint binding 가능하게 해준다함.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLocalAttackBoundaryFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemoteAttackBoundaryFinished);

UCLASS()
class ASSSLASH_API AAssslashCharacterAttackBoundary : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAssslashCharacterAttackBoundary();

	// 선언된 Delegate type 변수 선언. 이벤트를 블루프린트에도 노출시킨다.
	UPROPERTY(BlueprintAssignable, Category="Attack")
	FOnLocalAttackBoundaryFinished OnLocalAttackBoundaryFinished;

	UPROPERTY(BlueprintAssignable, Category="Attack")
	FOnLocalAttackBoundaryFinished OnRemoteAttackBoundaryFinished;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* AttackNiagaraSystem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetIsLocal(bool bNewIsLocal);

private:
	// 일정 시간 후 함수 호출하기 위한 타이머 핸들
	FTimerHandle DestroyTimerHandle;

	// 이건 매크로임.
	UPROPERTY(EditAnywhere, Category="Attack")
	float LifeTimeAfterSpawn = 1.f;

	UFUNCTION()
	void OnLifeSpanFinished();

	bool bIsLocal = false;
};
