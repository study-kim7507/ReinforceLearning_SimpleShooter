// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealPack.generated.h"

class UParticleSystem;

UCLASS()
class RL_SIMPLESHOOTER_API AHealPack : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HealPack", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HealPack")
	int32 HealAmount = 10;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HealParticle;
	
	
public:	
	// Sets default values for this actor's properties
	AHealPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
