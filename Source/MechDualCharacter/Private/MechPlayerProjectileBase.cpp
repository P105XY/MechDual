// Fill out your copyright notice in the Description page of Project Settings.


#include "MechPlayerProjectileBase.h"

// Sets default values
AMechPlayerProjectileBase::AMechPlayerProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMechPlayerProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMechPlayerProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

