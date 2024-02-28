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
	AMechPlayerProjectileBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision Particle Template")
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