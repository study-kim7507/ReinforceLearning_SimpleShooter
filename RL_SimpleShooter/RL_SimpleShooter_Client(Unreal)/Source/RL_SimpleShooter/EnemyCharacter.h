#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacterBase.h"
#include "EnemyCharacter.generated.h"


/*
	학습 시 플레이어 대신 에이전트의 상대가 되는 더미 캐릭터
	실제 플레이 시에는 사용되지 않음
*/


UCLASS()
class RL_SIMPLESHOOTER_API AEnemyCharacter : public AShooterCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};	
