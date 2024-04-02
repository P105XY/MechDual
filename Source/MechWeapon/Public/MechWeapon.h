#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMechDualWeapon, All, All)

class FMechWeaponModule : public IModuleInterface
{

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

