#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacterBase.h"
#include "EnemyCharacter.generated.h"

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
