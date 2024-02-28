// Fill out your copyright notice in the Description page of Project Settings.


#include "MechPlayerProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMechPlayerProjectileBase::AMechPlayerProjectileBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProjectileComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	ProjectileComponent->InitSphereRadius(37.5f);
	ProjectileComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));\

	RootComponent = ProjectileComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -37.5f));
	ProjectileMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;
}

// Called when the game starts or when spawned
void AMechPlayerProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		ProjectileComponent->OnComponentHit.AddDynamic(this, &AMechPlayerProjectileBase::OnProjectileImpact);
	}
}

// Called every frame
void AMechPlayerProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMechPlayerProjectileBase::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	FTransform HitTransform = FTransform();
	FVector HitLocation = GetActorLocation();
	HitTransform.SetLocation(HitLocation);
	HitTransform.SetRotation(FQuat::Identity);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffectTemplate, HitTransform);

	if (OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, HitLocation, Hit, GetInstigator()->Controller, this, DamageType);
	}

	Destroy();
}

