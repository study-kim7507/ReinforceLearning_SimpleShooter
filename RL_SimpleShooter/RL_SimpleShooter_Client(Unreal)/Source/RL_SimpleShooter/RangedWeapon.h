// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "RangedWeapon.generated.h"

class AShooterCharacterBase;
class AProjectile;
class UAnimSequence;

UCLASS()
class RL_SIMPLESHOOTER_API ARangedWeapon : public AWeaponBase
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AProjectile* Projectile;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimSequence* FireAnimation;

public:
	ARangedWeapon();

	UFUNCTION()
	void FireWithProjectile(AShooterCharacterBase* _Owner);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
