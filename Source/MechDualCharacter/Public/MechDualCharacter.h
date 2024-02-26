// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMechDual, All, All)

class FMechDualCharacterModule : public IModuleInterface
{

public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

