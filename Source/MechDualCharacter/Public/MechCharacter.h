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
class AMechPlayerProjectileBase;
class USkeletalMeshComponent;

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
	void FireWeapon(const FInputActionInstance& Instance);

private:
	void OnUpdateHealth();

protected:
	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	UFUNCTION(Server, Reliable)
	void HandleFire();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputMapping")
	UInputMappingContext* InputContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "Movement"))
	UInputAction* Movement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "LookAround"))
	UInputAction* LookAroundAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InputAction", meta = (DisplayName = "FireWeapon"))
	UInputAction* FireWeaponAction;
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<AMechPlayerProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	UPROPERTY()
	FTimerHandle FiringTimer;

public:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|Head")
	TObjectPtr<USkeletalMeshComponent> HeadMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|Arm")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|Body")
	TObjectPtr<USkeletalMeshComponent> BodyMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|Leg")
	TObjectPtr<USkeletalMeshComponent> LegMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|MainWeapon")
	TObjectPtr<USkeletalMeshComponent> MainWeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|SubWeapon")
	TObjectPtr<USkeletalMeshComponent> SubWeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|LeftShoulderWeapon")
	TObjectPtr<USkeletalMeshComponent> LeftShoulderWeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|RightShoulderWeapon")
	TObjectPtr<USkeletalMeshComponent> RightShoulderWeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerMesh|BoosterMesh")
	TObjectPtr<USkeletalMeshComponent> BoosterMesh;

protected:
	bool bIsFiringWeapon;

private:
	TObjectPtr<USpringArmComponent> CameraArm;
	TObjectPtr<UCameraComponent> PlayerCamera;

};
