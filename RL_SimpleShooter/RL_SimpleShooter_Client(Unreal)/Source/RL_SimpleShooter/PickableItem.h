// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickableItem.generated.h"

UCLASS()
class RL_SIMPLESHOOTER_API APickableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickableItem();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// virtual void PickedUp(class AShooterCharacterBase* Character);

	UPROPERTY(VisibleDefaultsOnly, Category = PickableItem)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = PickableItem)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = PickableItem)
	class USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = PickableItem)
	class URotatingMovementComponent* RotationComp;

	UPROPERTY(EditAnywhere, Category = PickableItem)
	class USoundBase* PickupSound;

	AActor* Item;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ItemClass;

private:
	

};
