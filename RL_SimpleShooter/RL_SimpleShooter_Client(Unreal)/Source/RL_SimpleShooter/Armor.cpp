// Fill out your copyright notice in the Description page of Project Settings.


#include "Armor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AArmor::AArmor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	ChangeArmorSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/AssetsPack/Armor/Change_Armor_Sound.Change_Armor_Sound"));
}

// Called when the game starts or when spawned
void AArmor::BeginPlay()
{
	Super::BeginPlay();
	if (ChangeArmorSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChangeArmorSound, GetActorLocation());
	}
}

// Called every frame
void AArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

