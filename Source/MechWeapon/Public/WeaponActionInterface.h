// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActionInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UWeaponActionInterface : public UInterface
{
	GENERATED_BODY()
};

class MECHWEAPON_API IWeaponActionInterface
{
	GENERATED_BODY()

public:
	virtual void WeaponAction();

};
