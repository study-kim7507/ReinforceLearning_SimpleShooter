// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ShooterCharacterBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 30000.f;
	ProjectileMovementComponent->InitialSpeed = 30000.f;
	
	// 속도에 따른 Rotation 변화
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = false;
	
	
	// 발사체의 수명 
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// 발사체의 부모는 무기.
	auto MyOwner = GetOwner();
	if (MyOwner == nullptr) return;

	// 무기의 부모는 무기를 소지한 캐릭터
	auto MyOwnerCharacter = MyOwner->GetOwner();
	if (MyOwnerCharacter == nullptr) return;

	auto MyOwnerCharacterInstigator = MyOwnerCharacter->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwnerCharacter)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerCharacterInstigator, this, DamageTypeClass);
		Destroy();
	}
}

void AProjectile::Fire(const FVector& Direction) const
{
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

