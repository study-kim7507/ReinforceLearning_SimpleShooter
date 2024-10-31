// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacterBase.h"
#include "OurAgentCharacter.generated.h"

/*
	모델에서 받은 최적의 행동을 수행하는 에이전트 캐릭터 (좀비 캐릭터)
	실제 플레이 시에는 플레이어가 상대하는 적 캐릭터가 된다 (좀비 캐릭터)
*/


UCLASS()
class RL_SIMPLESHOOTER_API AOurAgentCharacter : public AShooterCharacterBase
{
	GENERATED_BODY()

public:
	AOurAgentCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> PerformLineTraceFromEye(FVector CurrentLocation);

	UFUNCTION(BlueprintCallable)
	void TeleportToForwardDirection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void Rotate(float TargetView, float Duration);

	UFUNCTION(BlueprintCallable)
	void MoveForward(float Duration);


private:

	FRotator StartRotation;
	FRotator EndRotation;
	
	bool bIsRotating = false;
	float CurrentRotationTime = 0.0f;
	float TotalRotationTime = 0.0f;
	
	bool bIsMovingForward = false;
	float TotalMovementTime = 0.0f;
	float TravelTime = 0.0f;
};


