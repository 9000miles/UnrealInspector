// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

namespace UOBJECT_COLLECTOR { class FUObjectListener; }

class FUObjectCollectorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<UOBJECT_COLLECTOR::FUObjectListener> ObjectListener;
};