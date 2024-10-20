// Fill out your copyright notice in the Description page of Project Settings.


#include "HealPack.h"

// Sets default values
AHealPack::AHealPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHealPack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

