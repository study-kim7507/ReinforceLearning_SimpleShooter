// Fill out your copyright notice in the Description page of Project Settings.

#include "OurAgentCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AOurAgentCharacter::AOurAgentCharacter()
{

}

void AOurAgentCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize variables
	StartRotation = GetActorRotation();
	
	bIsRotating = false;
	CurrentRotationTime = 0.0f;
	TotalRotationTime = 0.0f;

	bIsMovingForward = false;
	TravelTime = 0.0f;
	TotalMovementTime = 0.f;
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

	if (bIsMovingForward)
	{
		TravelTime += DeltaTime;
		float MoveStep = GetCharacterMovement()->MaxWalkSpeed * DeltaTime;
		AddMovementInput(GetActorForwardVector(), MoveStep);
		if (TravelTime >= TotalMovementTime)
		{
			bIsMovingForward = false;
			TravelTime = 0.0f;
		}	
	}
}

/* For training and playing*/
// 0~15에 해당하는 방향으로 라인트레이스
TArray<FVector> AOurAgentCharacter::PerformLineTraceFromEye(FVector CurrentLocation)
{

	// 시야 판단을 위해 비교할 트레이스 채널
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);


	TArray<FVector> Result;

	for (int i = 0; i <= 15; i++)
	{
		float Angle;
		if (i <= 8)
			Angle = i * 22.5;
		else if (i <= 15)
			Angle = (i - 9) * 22.5 - 157.5;

		FVector Start = CurrentLocation;

		// 캐릭터가 Angle 방향으로 회전한 뒤, 2초동안 이동했을 때의 위치
		// 라인트레이스를 이용하여 해당 방향으로 2초 동안 이동할 경우 장애물에 부딪히는지 감지를 위함
		FVector End = Start + FRotator(0.0f, Angle, 0.0f).Vector() * GetCharacterMovement()->MaxWalkSpeed * 2.0f;

		// 캐릭터가 Angle 방향으로 회전한 뒤, 0.5초동안 이동했을 때의 위치
		FVector ForwardVector = Start + FRotator(0.0f, Angle, 0.0f).Vector() * GetCharacterMovement()->MaxWalkSpeed * 0.5f;

		FHitResult Hit;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);

		/* For debugging*/
		// DrawDebugLine(GetWorld(), Start, End, FColor::Red);
		// DrawDebugSphere(GetWorld(), End, 5.0f, 10, FColor::Emerald);
		// DrawDebugSphere(GetWorld(), ForwardVector, 5.0f, 10, FColor::Red);
		if (bHit)
		{
			// 만약, 캐릭터가 Angle 방향으로 회전한 뒤 1초동안 이동할 때, 장애물에 부딪히면 거리를 계산에서 제외 되도록 더미 값 반환 
			// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 25.f, 10, FColor::Blue);
			Result.Add(FVector(0.0f, 0.0f, 0.0f));
		}
		else
			Result.Add(ForwardVector);
	}

	return Result;
}


/* For training */
// 학습 시 모델에서 받아온 Action값을 시뮬레이션 하는 시간을 줄이기 위함
void AOurAgentCharacter::TeleportToForwardDirection()
{
	FVector CurrentLocation = GetActorLocation();
	FVector GoalLocation = CurrentLocation + GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed * 0.5f;
	this->SetActorLocation(GoalLocation);
}


void AOurAgentCharacter::Rotate(float TargetView, float Duration)
{
	// 지정된 시간동안 특정 각도를 향해 회전
	EndRotation = StartRotation + FRotator(0.0f, TargetView, 0.0f);
	TotalRotationTime = Duration;
	CurrentRotationTime = 0.0f;
	bIsRotating = true;
}

void AOurAgentCharacter::MoveForward(float Duration)
{
	// 지정된 시간만큼 앞으로 이동
	bIsMovingForward = true;
	TotalMovementTime = Duration;
}



