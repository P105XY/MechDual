// Fill out your copyright notice in the Description page of Project Settings.


#include "MechCharacter.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "InputTriggers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MechDualCharacter/Public/MechPlayerController.h"

// Sets default values
AMechCharacter::AMechCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
}

// Called when the game starts or when spawned
void AMechCharacter::BeginPlay()
{
	Super::BeginPlay();
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

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	subSystem->ClearAllMappings();
	subSystem->AddMappingContext(InputContext, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	PEI->BindAction(MoveFront, ETriggerEvent::Triggered, this, &AMechCharacter::MovementFront);
	PEI->BindAction(MoveSide, ETriggerEvent::Triggered, this, &AMechCharacter::MovementSide);
	PEI->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AMechCharacter::LookAround);
}

void AMechCharacter::MovementFront(const FInputActionValue& Value)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, PlayerController->GetControlRotation().Yaw, 0);

		// Forward/Backward direction
		if (MoveValue.Y != 0.f)
		{
			// Get forward vector
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);

			AddMovementInput(Direction, MoveValue.Y);
		}
	}
}

void AMechCharacter::MovementSide(const FInputActionValue& Value)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, PlayerController->GetControlRotation().Yaw, 0);

		// Right/Left direction
		if (MoveValue.X != 0.f)
		{
			// Get right vector
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

void AMechCharacter::LookAround(const FInputActionValue& Value)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		const FVector2D lookValue = Value.Get<FVector2D>();
		if (lookValue.X != 0)
		{
			AddControllerPitchInput(-lookValue.X);
		}
		if (lookValue.Y != 0)
		{
			AddControllerYawInput(lookValue.Y);
		}
	}
}

