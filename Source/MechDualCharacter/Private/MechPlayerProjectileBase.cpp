// Fill out your copyright notice in the Description page of Project Settings.


#include "MechPlayerProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMechPlayerProjectileBase::AMechPlayerProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProjectileComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	ProjectileComponent->InitSphereRadius(37.5f);
	ProjectileComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	ProjectileComponent->MoveIgnoreActors.Add(GetOwner());

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
		ProjectileComponent->OnComponentBeginOverlap.AddDynamic(this, &AMechPlayerProjectileBase::OnProjectileImpact);
	}
}

// Called every frame
void AMechPlayerProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMechPlayerProjectileBase::OnProjectileImpact(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FTransform HitTransform = FTransform();
	FVector HitLocation = GetActorLocation();
	HitTransform.SetLocation(HitLocation);
	HitTransform.SetRotation(FQuat::Identity);

	if (OtherActor->IsA(AMechPlayerProjectileBase::StaticClass()))
	{
		return;
	}

	if (OtherActor == GetInstigator())
	{
		return;
	}

	if (OtherActor == GetOwner())
	{
		return;
	}

	if (OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffectTemplate, HitTransform);
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, HitLocation, SweepResult, GetInstigator()->Controller, this, DamageType);
	}

	Destroy();
}

