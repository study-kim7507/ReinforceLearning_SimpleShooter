// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacterBase.h"
#include "OurAgentCharacter.generated.h"

UCLASS()
class RL_SIMPLESHOOTER_API AOurAgentCharacter : public AShooterCharacterBase
{
	GENERATED_BODY()

public:
	AOurAgentCharacter();

	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void Rotate(float TargetView, float Duration);

	UFUNCTION(BlueprintCallable)
	void MoveForward(float Duration);

	UFUNCTION(BlueprintCallable)
	void InitializationVariables();

private:
	bool bIsRotating = false;
	float CurrentRotationTime = 0.0f;
	float TotalRotationTime = 0.0f;
	FRotator StartRotation;
	FRotator EndRotation;

	bool bMoveForward = false;
	float TotalMovementTime = 0.0f;
	float TravelTime = 0.0f;
};


