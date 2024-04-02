// Fill out your copyright notice in the Description page of Project Settings.


#include "MechCharacter.h"
#include "Engine/DataAsset.h"
#include "Engine/LocalPlayer.h"
#include "InputActionValue.h"
#include "InputTriggers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MechDualCharacter/Public/MechPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MechCharacterMovementComponent.h"
#include "MechPlayerProjectileBase.h"
#include "Engine/World.h"

// Sets default values
AMechCharacter::AMechCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMechCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerCameraArm"));
	CameraArm->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	CameraArm->TargetArmLength = 350.0f;
	CameraArm->SocketOffset = FVector(-125.0f, 0.0f, 150.0f);
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 5.0f;
	CameraArm->bInheritPitch = true;
	CameraArm->bInheritYaw = true;
	CameraArm->bInheritRoll = true;
	CameraArm->bUsePawnControlRotation = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	MechCharacterMovementComponent = Cast<UMechCharacterMovementComponent>(GetCharacterMovement());
	MechCharacterMovementComponent->bOrientRotationToMovement = true;
	MechCharacterMovementComponent->bUseControllerDesiredRotation = false;

	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmMesh"));
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	MainWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MainWeaponMesh"));
	SubWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubWeaponMesh"));
	LeftShoulderWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftShoulderWeaponMesh"));
	RightShoulderWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightShoulderWeaponMesh"));
	BoosterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BoosterMesh"));
}

// Called when the game starts or when spawned
void AMechCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputContext, 0);
}

// Called every frame
void AMechCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMechCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (PEI == nullptr)
	{
		return;
	}

	PEI->BindAction(Movement, ETriggerEvent::Triggered, this, &AMechCharacter::Movment);
	PEI->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AMechCharacter::LookAround);
	PEI->BindAction(FireWeaponAction, ETriggerEvent::Triggered, this, &AMechCharacter::FireWeapon);
}

void AMechCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMechCharacter, CurrentHealth);
}

void AMechCharacter::Movment(const FInputActionInstance& Instance)
{
	FVector2D MovementVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMechCharacter::LookAround(const FInputActionInstance& Instance)
{
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void AMechCharacter::FireWeapon(const FInputActionInstance& Instance)
{
	StartFire();
}

void AMechCharacter::OnUpdateHealth()
{
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}
}

float AMechCharacter::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(DamageApplied);
	return DamageApplied;
}

void AMechCharacter::OnRep_CurrentHealth()
{
	OnUpdateHealth();
}

void AMechCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &AMechCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

void AMechCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void AMechCharacter::HandleFire_Implementation()
{
	FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AMechPlayerProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AMechPlayerProjectileBase>(ProjectileClass, spawnLocation, spawnRotation, spawnParameters);
}
