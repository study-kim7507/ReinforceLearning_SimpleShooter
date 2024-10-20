// Fill out your copyright notice in the Description page of Project Settings.

#include "OurAgentCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AOurAgentCharacter::AOurAgentCharacter()
{
	InitializationVariables();
}

void AOurAgentCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize variables
	StartRotation = GetActorRotation();
	bIsRotating = false;
	CurrentRotationTime = 0.0f;
	TotalRotationTime = 0.0f;
}

void AOurAgentCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRotating)
	{
		CurrentRotationTime += DeltaTime;
		float Alpha = CurrentRotationTime / TotalRotationTime;

		if (Alpha >= 1.0f)
		{
			Alpha = 1.0f;
			bIsRotating = false;
		}

		FRotator NewRotation = FMath::Lerp(GetActorRotation() , EndRotation, Alpha);

		SetActorRotation(NewRotation);
	}

	if (bMoveForward && !bIsRotating)
	{
		TravelTime += DeltaTime;
		float MoveStep = GetCharacterMovement()->MaxWalkSpeed * DeltaTime;
		AddMovementInput(GetActorForwardVector(), MoveStep);
		if (TravelTime >= TotalMovementTime)
		{
			bMoveForward = false;
			TravelTime = 0.0f;
		}	
	}
}



void AOurAgentCharacter::Rotate(float TargetView, float Duration)
{
	// Set up rotation
	EndRotation = StartRotation + FRotator(0.0f, TargetView, 0.0f);
	TotalRotationTime = Duration;
	CurrentRotationTime = 0.0f;
	bIsRotating = true;
}

void AOurAgentCharacter::MoveForward(float Duration)
{
	bMoveForward = true;
	TotalMovementTime = Duration;
}

void AOurAgentCharacter::InitializationVariables()
{
	// Initialize variables
	bIsRotating = false;
	CurrentRotationTime = 0.0f;
	TotalRotationTime = 0.0f;

	bMoveForward = false;
}


