// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "EngineUtils.h"
#include "AIController.h"
#include "ShooterCharacterBase.h"
#include "PlayerCharacterController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

void AMyGameModeBase::ActorDied(AActor* DeadActor)
{
	AShooterCharacterBase* DeadCharacter = Cast<AShooterCharacterBase>(DeadActor);
	if (DeadCharacter == nullptr) return;

	// 만약 죽은 캐릭터가 플레이어 캐릭터인 경우 게임 패배
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(DeadCharacter->GetController());
	if (PlayerController != nullptr)
	{
		EndGame(false);
	}

	// 모든 좀비(에이전트) 캐릭터가 죽으면 게임 승리
	AAIController* OurAgentAIController = Cast<AAIController>(DeadCharacter->GetController());
	if (OurAgentAIController != nullptr)
	{
		TArray<AActor*> OurAgentAIControllers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIController::StaticClass(), OurAgentAIControllers);
		
		int32 OurAgentAICount = OurAgentAIControllers.Num();
		OurAgentAICount--;
		
		if (OurAgentAICount == 0)
		{
			EndGame(true);	
		}
		
	}
	DeadCharacter->HandleDestruction();
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AShooterCharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AMyGameModeBase::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}