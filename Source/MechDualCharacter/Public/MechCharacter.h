// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MechCharacter.generated.h"

struct FInputActionValue;
class AMechPlayerController;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MECHDUALCHARACTER_API AMechCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMechCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MovementFront(const FInputActionValue& Value);
	void MovementSide(const FInputActionValue& Value);
	void LookAround(const FInputActionValue& Value);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputMapping")
	UInputMappingContext* InputContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "MovementFront"))
	UInputAction* MoveFront;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "MovementSide"))
	UInputAction* MoveSide;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "LookAround"))
	UInputAction* LookAroundAction;

private:
	TObjectPtr<USpringArmComponent> CameraArm;
	TObjectPtr<UCameraComponent> PlayerCamera;
	
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;
};
