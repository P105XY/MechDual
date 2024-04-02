// Fill out your copyright notice in the Description page of Project Settings.


#include "MechWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

AMechWeaponBase::AMechWeaponBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

}

void AMechWeaponBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AMechWeaponBase::WeaponAction()
{
}
