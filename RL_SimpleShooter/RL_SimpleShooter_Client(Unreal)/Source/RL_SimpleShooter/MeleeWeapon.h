// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "MeleeWeapon.generated.h"


class AShooterCharacterBase;
/**
 * 
 */
UCLASS()
class RL_SIMPLESHOOTER_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AMeleeWeapon();

	UFUNCTION()
	void AttackUsingMeleeWeapon(AShooterCharacterBase* _Owner);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	float Damage = 30.0f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
