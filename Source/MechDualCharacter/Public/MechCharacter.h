// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE void SetHealth(float NewHealth) { MaxHealth = NewHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE void SetCurrentHealth(float NewHealth)
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
			OnUpdateHealth();
		}
	}

public:
	float TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

protected:
	void Movment(const FInputActionInstance& Instance);
	void LookAround(const FInputActionInstance& Instance);

private:
	void OnUpdateHealth();

protected:
	UFUNCTION()
	void OnRep_CurrentHealth();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputMapping")
	UInputMappingContext* InputContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "Movement"))
	UInputAction* Movement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "LookAround"))
	UInputAction* LookAroundAction;
	
private:
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComponent;

	UPROPERTY()
	UMechCharacterMovementComponent* MechCharacterMovementComponent;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentHealth, Category = "Health")
	float CurrentHealth;

private:
	TObjectPtr<USpringArmComponent> CameraArm;
	TObjectPtr<UCameraComponent> PlayerCamera;

};
