// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActionInterface.h"
#include "MechWeaponBase.generated.h"

UENUM()
enum class EWeaponType : int8
{
	Rifle UMETA(DisplayName = "Rifle"),
	Launcher UMETA(DisplayName = "Launcher"),
	Meelee UMETA(DisplayName = "Meelee"),
	Missle UMETA(DisplayName = "Missle"),
	ChargeableLauncher UMETA(DisplayName = "ChargeableLauncher"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	MarksmanRifle UMETA(DisplayName = "MarksmanRifle"),
	Pistol UMETA(DIsplayName = "Pistol")
};

class USkeletalMeshComponent;

/*
* 플레이어 Mech가 사용할 무기 클래스.
*/
UCLASS()
class MECHWEAPON_API AMechWeaponBase : public AActor, public IWeaponActionInterface
{
	GENERATED_BODY()
	
public:	
	AMechWeaponBase(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void WeaponAction() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|WeaponMesh")
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|WeaponType")
	EWeaponType WeaponType;

private:
	bool bIsClicked;

};
