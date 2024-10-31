// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedWeapon.h"
#include "ShooterCharacterBase.h"
#include "Projectile.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

ARangedWeapon::ARangedWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARangedWeapon::FireWithProjectile(AShooterCharacterBase* _Owner)
{
	if (_Owner == nullptr || _Owner->GetController() == nullptr) return;
	const FVector StartLocation = ProjectileSpawnPoint->GetComponentLocation();
	const FVector EndLocation = _Owner->GetController()->GetControlRotation().Vector() + StartLocation;
	FVector Direction = EndLocation - StartLocation;

	if (ProjectileClass != nullptr)
	{
		Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass);
		if (Projectile != nullptr)
		{
			Projectile->SetOwner(this);
			Projectile->SetActorLocation(StartLocation);
			Projectile->SetActorRotation(Direction.Rotation());
			Projectile->AddActorLocalRotation(FRotator(-90, 0, 0));

			if (Direction.Normalize())
			{
				if (FireAnimation != nullptr)
				{
					Mesh->PlayAnimation(FireAnimation, false);
				}
				APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(_Owner);
				if (PlayerCharacter != nullptr)
				{
					if (PlayerCharacter->FireCameraShakeClass != nullptr)
					{
						GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(PlayerCharacter->FireCameraShakeClass);
					}
				}
				Projectile->Fire(Direction);
			}
		}
	}
}