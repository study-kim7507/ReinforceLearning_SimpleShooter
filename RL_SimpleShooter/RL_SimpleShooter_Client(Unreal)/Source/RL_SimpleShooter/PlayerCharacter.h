// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacterBase.h"
#include "PlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UCameraShakeBase;
struct FInputActionValue;

UCLASS()
class RL_SIMPLESHOOTER_API APlayerCharacter : public AShooterCharacterBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArm;

public:
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* IA_Attack;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* IA_CycleWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* IA_CycleArmor;

	/*
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* IA_CameraMode;
	*/

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> FireCameraShakeClass;

private:
	// 플레이어의 입력을 처리하기 위한 함수 구현
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void CycleWeapon();
	void CycleArmor();

};
