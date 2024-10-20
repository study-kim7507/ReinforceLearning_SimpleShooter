// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RL_SIMPLESHOOTER_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

private:
	class AShooterCharacterBase* PlayerCharacter;

	// TODO: 현재 구현은 플레이어가 이겼을 경우만 다룸.
	// 추후 학습 대상(에이전트)가 이겼을 경우도 다룰 수 있도록 변경해야함.
	void EndGame(bool bIsPlayerWinner);
};
