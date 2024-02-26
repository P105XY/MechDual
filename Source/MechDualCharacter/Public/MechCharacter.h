// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MechCharacter.generated.h"

struct FInputActionValue;
struct FInputActionInstance;
class AMechPlayerController;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UMechCharacterMovementComponent;

UCLASS()
class MECHDUALCHARACTER_API AMechCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMechCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Movment(const FInputActionInstance& Instance);
	void LookAround(const FInputActionInstance& Instance);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputMapping")
	UInputMappingContext* InputContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "Movement"))
	UInputAction* Movement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "LookAround"))
	UInputAction* LookAroundAction;

private:
	TObjectPtr<USpringArmComponent> CameraArm;
	TObjectPtr<UCameraComponent> PlayerCamera;
	
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;

	UPROPERTY()
	UMechCharacterMovementComponent* MechCharacterMovementComponent;
};
