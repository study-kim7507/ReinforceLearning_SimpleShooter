// Fill out your copyright notice in the Description page of Project Settings.

#include "ShooterCharacterBase.h"
#include "WeaponBase.h"
#include "Armor.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RangedWeapon.h"
#include "MeleeWeapon.h"
#include "HealPack.h"
#include "PowerUp.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AShooterCharacterBase::AShooterCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	ChangeStateEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	ChangeStateEffect->SetupAttachment(GetMesh(), TEXT("Hips"));
	ChangeStateEffect->bAutoActivate = true;
}

void AShooterCharacterBase::HandleDestruction()
{
	// 죽은 캐릭터의 차일드 액터 모두 소멸
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		Actor->Destroy();
	}

	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// 새로운 아이템 획득
void AShooterCharacterBase::GetNewItem(TSubclassOf<class AActor> NewItemClass)
{
	if (NewItemClass->IsChildOf(AWeaponBase::StaticClass()))
	{
		TSubclassOf<AWeaponBase> NewWeapon = Cast<UClass>(NewItemClass);
		if (NewWeapon != nullptr)
			HeldWeapons.Add(NewWeapon);
	}
	else if (NewItemClass->IsChildOf(AArmor::StaticClass()))
	{
		TSubclassOf<AArmor> NewArmor = Cast<UClass>(NewItemClass);
		if (NewArmor != nullptr)
			HeldArmors.Add(NewArmor);
	}
	else if (NewItemClass->IsChildOf(AHealPack::StaticClass()))
	{
		TSubclassOf<AHealPack> NewHealPack = Cast<UClass>(NewItemClass);
		AHealPack* HealPackInstance = GetWorld()->SpawnActor<AHealPack>(NewHealPack, GetActorTransform());
		if (HealPackInstance)
		{
			// Mesh의 Visibility를 끔
			HealPackInstance->Mesh->SetVisibility(false);

			// Mesh의 Collision을 비활성화
			HealPackInstance->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if (HealPackInstance->HealParticle != nullptr)
			{
				ChangeStateEffect->SetTemplate(HealPackInstance->HealParticle);
				ChangeStateEffect->Activate();
			}

			// HealAmount 값 불러오기
			this->HealthComponent->CurrentHealth += HealPackInstance->HealAmount;
			if (this->HealthComponent->CurrentHealth > 100.0f)
				this->HealthComponent->CurrentHealth = 100.0f;

			// 인스턴스 소멸 (필요하다면)
			HealPackInstance->Destroy();
		}
	}
	else if (NewItemClass->IsChildOf(APowerUp::StaticClass()))
	{
		TSubclassOf<APowerUp> NewPowerUp = Cast<UClass>(NewItemClass);
		APowerUp* PowerUpInstance = GetWorld()->SpawnActor<APowerUp>(NewPowerUp, GetActorTransform());
		if (PowerUpInstance)
		{
			// Mesh의 Visibility를 끔
			PowerUpInstance->Mesh->SetVisibility(false);

			// Mesh의 Collision을 비활성화
			PowerUpInstance->Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			if (PowerUpInstance->PowerUpParticle != nullptr)
			{
				ChangeStateEffect->SetTemplate(PowerUpInstance->PowerUpParticle);
				ChangeStateEffect->Activate();
			}

			// PowerUpAmount 값 불러오기
			this->CurrentPower = PowerUpInstance->PowerUpAmount;

			// 인스턴스 소멸 (필요하다면)
			PowerUpInstance->Destroy();
		}
	}
}

// 0~15에 해당하는 방향으로 라인트레이스
TArray<FVector> AShooterCharacterBase::PerformLineTraceFromEye()
{
	if (!bIsPerfromLineTrace) return TArray<FVector>();

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

		FVector Start = GetActorLocation();

		// 캐릭터가 Angle 방향으로 회전한 뒤, 1초동안 이동했을 때의 위치
		// 라인트레이스를 이용하여 해당 방향으로 1초 동안 이동할 경우 장애물에 부딪히는지 감지를 위함
		FVector End = Start + FRotator(0.0f, Angle, 0.0f).Vector() * GetCharacterMovement()->MaxWalkSpeed;	
		
		// 캐릭터가 Angle 방향으로 회전한 뒤, 0.5초동안 이동했을 때의 위치
		FVector ForwardVector = Start + FRotator(0.0f, Angle, 0.0f).Vector() * GetCharacterMovement()->MaxWalkSpeed * 0.5f;

		FHitResult Hit;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);		

		DrawDebugLine(GetWorld(), Start, End, FColor::Red);
		DrawDebugSphere(GetWorld(), End, 5.0f, 10, FColor::Emerald);
		DrawDebugSphere(GetWorld(), ForwardVector, 5.0f, 10, FColor::Red);
		if (bHit)
		{
			// 만약, 캐릭터가 Angle 방향으로 회전한 뒤 1초동안 이동할 때, 장애물에 부딪히면 거리를 계산에서 제외 되도록 더미 값 반환 
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 25.f, 10, FColor::Blue);
			Result.Add(FVector(0.0f, 0.0f, 0.0f));
		}
		else
			Result.Add(ForwardVector);
	}

	return Result;
}

bool AShooterCharacterBase::IsPathBlocked()
{
	FVector Start = GetMesh()->GetSocketLocation(TEXT("Spine"));
	FVector End = Start + GetActorForwardVector() * 5000.f;

	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(ECC_WorldStatic);

	FHitResult Hit;

	DrawDebugLine(GetWorld(), Start, End, FColor::Blue);
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 50.f, 50, FColor::Green);
		return true;
	}

	return false;
}

// Called when the game starts or when spawned
void AShooterCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (HeldWeapons.Num() > 0)
	{
		CurrentWeaponNum = 0;
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(HeldWeapons[CurrentWeaponNum]);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		CurrentWeapon->SetOwner(this);
	}
	else
	{
		CurrentWeaponNum = -1;
		CurrentWeapon = nullptr;
	}
	
	if (HeldArmors.Num() > 0)
	{
		CurrentArmorNum = 0;
		CurrentArmor = GetWorld()->SpawnActor<AArmor>(HeldArmors[CurrentArmorNum]);
		CurrentArmor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ArmorSocket"));
		CurrentArmor->SetOwner(this);
	}
	else
	{
		CurrentArmorNum = -1;
		CurrentArmor = nullptr;
	}

}

// Called every frame
void AShooterCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// PerformLineTraceFromEye();
	// IsPathBlocked();
}

// Called to bind functionality to input
void AShooterCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AShooterCharacterBase::Attack()
{
	if (CurrentWeapon != nullptr)
	{
		ARangedWeapon* CurrentRangedWeapon = Cast<ARangedWeapon>(CurrentWeapon);
		if (CurrentRangedWeapon != nullptr)
		{
			CurrentRangedWeapon->FireWithProjectile(this);
			OnPullTriggerEvent.Broadcast();
		}	
		AMeleeWeapon* CurrentMeleeWeapon = Cast<AMeleeWeapon>(CurrentWeapon);
		if (CurrentMeleeWeapon != nullptr)
		{
			CurrentMeleeWeapon->AttackUsingMeleeWeapon(this);
			bIsAttackUsingMeleeWeapon = true;
		}
	}
}


// 지정한 번호의 무기로 변경
void AShooterCharacterBase::ChangeWeapon(int32 TargetWeaponNum)
{
	AWeaponBase* PreWeapon = nullptr;
	if (CurrentWeapon != nullptr)
		PreWeapon = CurrentWeapon;

	if (HeldWeapons.Num() > 0)
	{
		CurrentWeaponNum = TargetWeaponNum;
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(HeldWeapons[CurrentWeaponNum]);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		CurrentWeapon->SetOwner(this);

		if (PreWeapon != nullptr) 
			PreWeapon->Destroy();
	}
}

// 지정한 번호의 방어구로 변경
void AShooterCharacterBase::ChangeArmor(int32 TargetArmorNum)
{
	AArmor* PreArmor = nullptr;
	if (CurrentArmor != nullptr)
		PreArmor = CurrentArmor;

	if (HeldArmors.Num() > 0)
	{
		CurrentArmorNum = TargetArmorNum;
		CurrentArmor = GetWorld()->SpawnActor<AArmor>(HeldArmors[CurrentArmorNum]);
		CurrentArmor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ArmorSocket"));
		CurrentArmor->SetOwner(this);
		if (PreArmor != nullptr) PreArmor->Destroy();
	}
}


