// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"
#include "Components/SphereComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h" 
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponBase.h"
#include "Armor.h"
#include "HealPack.h"
#include "PowerUp.h"
#include "PlayerCharacter.h"
#include "OurAgentCharacter.h"

// Sets default values
APickableItem::APickableItem()
{
	// 충돌 컴포넌트 생성 및 설정
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(30.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
	RootComponent = CollisionComp;

	// 스태틱 메시 컴포넌트 생성
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	// 스켈레탈 메시 컴포넌트 생성
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	RotationComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotationComp"));
}

void APickableItem::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		Player->GetNewItem(ItemClass);
	}
	else if (AOurAgentCharacter* Agent = Cast<AOurAgentCharacter>(OtherActor))
	{
		TArray<AActor*> TaggedActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Agent"), TaggedActors);
		for (AActor* Actor : TaggedActors)
		{
			AOurAgentCharacter* AgentCharacter = Cast<AOurAgentCharacter>(Actor);
			if (AgentCharacter != nullptr)
			{
				AgentCharacter->GetNewItem(ItemClass);
			}
		}
	}
	Destroy();
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickableItem::BeginOverlap);

	if (ItemClass != nullptr)
	{
		AActor* DefaultObject = ItemClass->GetDefaultObject<AActor>();
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(DefaultObject))
		{
			USkeletalMeshComponent* WeaponMesh = Weapon->FindComponentByClass<USkeletalMeshComponent>();
			if (WeaponMesh)
			{
				SkeletalMeshComp->SetSkeletalMesh(WeaponMesh->SkeletalMesh);
				SkeletalMeshComp->SetVisibility(true);
				StaticMeshComp->SetVisibility(false);
			}
		}
		// Armor 클래스인 경우
		else if (AArmor* Armor = Cast<AArmor>(DefaultObject))
		{
			// StaticMesh로 설정
			UStaticMeshComponent* ArmorMesh = Armor->FindComponentByClass<UStaticMeshComponent>();
			if (ArmorMesh)
			{
				StaticMeshComp->SetStaticMesh(ArmorMesh->GetStaticMesh());
				StaticMeshComp->SetVisibility(true);
				SkeletalMeshComp->SetVisibility(false);

				// 메시의 Bounds 기반 스케일 조정
				FBoxSphereBounds Bounds = StaticMeshComp->CalcBounds(StaticMeshComp->GetComponentTransform());
				float DesiredSize = 500.0f; // 원하는 크기
				float ScaleFactor = DesiredSize / Bounds.GetBox().GetSize().Size();
				StaticMeshComp->SetRelativeScale3D(FVector(ScaleFactor));
				
				StaticMeshComp->RegisterComponent();
			}
		}
		else if (AHealPack* HealPack = Cast<AHealPack>(DefaultObject))
		{
			// StaticMesh로 설정
			UStaticMeshComponent* HealPackMesh = HealPack->FindComponentByClass<UStaticMeshComponent>();
			if (HealPackMesh)
			{
				StaticMeshComp->SetStaticMesh(HealPackMesh->GetStaticMesh());
				StaticMeshComp->SetVisibility(true);
				SkeletalMeshComp->SetVisibility(false);

				// 메시의 Bounds 기반 스케일 조정
				FBoxSphereBounds Bounds = StaticMeshComp->CalcBounds(StaticMeshComp->GetComponentTransform());
				float DesiredSize = 500.0f; // 원하는 크기
				float ScaleFactor = DesiredSize / Bounds.GetBox().GetSize().Size();
				StaticMeshComp->SetRelativeScale3D(FVector(ScaleFactor));

				StaticMeshComp->RegisterComponent();
			}
		}
		else if (APowerUp* PowerUp = Cast<APowerUp>(DefaultObject))
		{
			// StaticMesh로 설정
			UStaticMeshComponent* PowerUpMesh = PowerUp->FindComponentByClass<UStaticMeshComponent>();
			if (PowerUpMesh)
			{
				StaticMeshComp->SetStaticMesh(PowerUpMesh->GetStaticMesh());
				StaticMeshComp->SetVisibility(true);
				SkeletalMeshComp->SetVisibility(false);

				// 메시의 Bounds 기반 스케일 조정
				FBoxSphereBounds Bounds = StaticMeshComp->CalcBounds(StaticMeshComp->GetComponentTransform());
				float DesiredSize = 500.0f; // 원하는 크기
				float ScaleFactor = DesiredSize / Bounds.GetBox().GetSize().Size();
				StaticMeshComp->SetRelativeScale3D(FVector(ScaleFactor));

				StaticMeshComp->RegisterComponent();
			}
		}
	}
}


