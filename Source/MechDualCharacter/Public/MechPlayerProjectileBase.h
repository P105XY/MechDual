// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MechPlayerProjectileBase.generated.h"

class UDamageType;
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class MECHDUALCHARACTER_API AMechPlayerProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMechPlayerProjectileBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnProjectileImpact(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GamePlay|Collision Particle Template")
	TObjectPtr<UParticleSystem> ExplosionEffectTemplate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision Component")
	TObjectPtr<USphereComponent> ProjectileComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision Particle")
	TObjectPtr<UParticleSystemComponent> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ProjectileMovement")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	TSubclassOf<UDamageType> DamageType;
	float Damage;
};