#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacterBase.generated.h"

class AWeaponBase;
class AArmor;
class UHealthComponent;
class UEyeLineTraceComponent;

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
	TArray<TSubclassOf<AWeaponBase>> HeldWeapons;						// 캐릭터가 현재 소지 중인 무기들

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	TArray<TSubclassOf<AArmor>> HeldArmors;								// 캐릭터가 현재 소지 중인 방어구들

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;									// 캐릭터의 체력을 관리하는 컴포넌트 

public:

	// 현재 캐릭터가 장착 중인 장비들
	AWeaponBase* CurrentWeapon;					// 현재 캐릭터가 장착 중인 무기
	AArmor* CurrentArmor;						// 현재 캐릭터가 장착 중인 방어구

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* HitSound;						// 피격 시 재생되는 사운드

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* DeathSound;						// 사망 시 재생되는 사운드

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsAttackUsingMeleeWeapon = false;		// 근접 무기 
};
