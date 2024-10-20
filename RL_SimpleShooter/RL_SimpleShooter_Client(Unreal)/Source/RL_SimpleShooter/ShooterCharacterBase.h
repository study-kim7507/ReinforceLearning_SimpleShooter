#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacterBase.generated.h"

class AWeaponBase;
class AArmor;
class UHealthComponent;
class UEyeLineTraceComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPullTriggerEvent);


UCLASS()
class RL_SIMPLESHOOTER_API AShooterCharacterBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY();
	class UParticleSystemComponent* ChangeStateEffect;

public:
	// Sets default values for this character's properties
	AShooterCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void ChangeWeapon(int32 TargetWeaponNum);

	UFUNCTION(BlueprintCallable)
	void ChangeArmor(int32 TargetArmorNum);

	UFUNCTION(BlueprintCallable)
	
	void Attack();

	void HandleDestruction();
	void GetNewItem(TSubclassOf<AActor> NewItemClass);

	UFUNCTION(BlueprintCallable)
	TArray<FVector> PerformLineTraceFromEye();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsPerfromLineTrace = true;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsPathBlocked();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPullTriggerEvent OnPullTriggerEvent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentWeaponNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentArmorNum = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentPower = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TArray<TSubclassOf<AWeaponBase>> HeldWeapons;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TArray<TSubclassOf<AArmor>> HeldArmors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

public:
	AWeaponBase* CurrentWeapon;
	AArmor* CurrentArmor;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HitSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttackUsingMeleeWeapon = false;
};
