// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Armor.h"
#include "WeaponBase.h"

APlayerCharacter::APlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(-135.0f, 30.0f, 75.0f));
	SpringArm->TargetArmLength = 600.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, "Camera");
	FollowCamera->bUsePawnControlRotation = false;
	bUseControllerRotationYaw = true;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedPlayerInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedPlayerInputComponent != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController != nullptr)
		{
			UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (EnhancedSubsystem != nullptr)
			{
				EnhancedSubsystem->AddMappingContext(IMC_Player, 1);
				EnhancedPlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
				EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
				EnhancedPlayerInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
				EnhancedPlayerInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
				EnhancedPlayerInputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
				EnhancedPlayerInputComponent->BindAction(IA_CycleWeapon, ETriggerEvent::Started, this, &APlayerCharacter::CycleWeapon);
				EnhancedPlayerInputComponent->BindAction(IA_CycleArmor, ETriggerEvent::Started, this, &APlayerCharacter::CycleArmor);
			}
		}
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (Controller != nullptr && (InputValue.X != 0.0f || InputValue.Y != 0.0f))
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

		if (InputValue.X != 0.0f)
		{
			const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);
			AddMovementInput(RightDirection, InputValue.X);
		}

		if (InputValue.Y != 0.0f)
		{
			const FVector ForwardDirection = YawRotation.Vector();
			AddMovementInput(ForwardDirection, InputValue.Y);
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (InputValue.X != 0.0f)
	{
		AddControllerYawInput(InputValue.X);
	}

	if (InputValue.Y != 0.0f)
	{
		AddControllerPitchInput(InputValue.Y);
	}

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::CycleWeapon()
{
	AWeaponBase* PreWeapon = nullptr;
	if (CurrentWeapon != nullptr)
		PreWeapon = CurrentWeapon;

	// 하나 이상의 무기를 소지하고 있는 경우
	if (HeldWeapons.Num() > 0)
	{
		CurrentWeaponNum++;
		if (CurrentWeaponNum >= HeldWeapons.Num()) CurrentWeaponNum = 0;
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(HeldWeapons[CurrentWeaponNum]);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		CurrentWeapon->SetOwner(this);
		if (PreWeapon != nullptr) PreWeapon->Destroy();
	}
}


void APlayerCharacter::CycleArmor()
{
	AArmor* PreArmor = nullptr;
	if (CurrentArmor != nullptr)
		PreArmor = CurrentArmor;

	// 하나 이상의 방어구를 소지하고 있는 경우
	if (HeldArmors.Num() != 0)
	{
		CurrentArmorNum++;
		if (CurrentArmorNum >= HeldArmors.Num()) CurrentArmorNum = 0;
		CurrentArmor = GetWorld()->SpawnActor<AArmor>(HeldArmors[CurrentArmorNum]);
		CurrentArmor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("ArmorSocket"));
		CurrentArmor->SetOwner(this);
		if (PreArmor != nullptr) PreArmor->Destroy();
	}
	
}