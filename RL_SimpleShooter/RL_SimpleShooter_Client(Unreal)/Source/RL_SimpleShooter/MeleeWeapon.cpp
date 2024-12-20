// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeWeapon.h"
#include "ShooterCharacterBase.h"
#include "Kismet/GameplayStatics.h"

AMeleeWeapon::AMeleeWeapon()
{
}

void AMeleeWeapon::AttackUsingMeleeWeapon(AShooterCharacterBase* _Owner)
{

}

void AMeleeWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto MyOwnerCharacter = GetOwner();
	if (MyOwnerCharacter == nullptr) return;

	auto MyOwnerCharacterInstigator = MyOwnerCharacter->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwnerCharacter)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerCharacterInstigator, this, DamageTypeClass);
	}
}

void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnOverlapBegin);
}

void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
