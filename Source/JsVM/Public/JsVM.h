// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "JsEnv.h"

class FJsVMModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnStartGameInstance(UGameInstance* GameInstance);
	TSharedPtr<PUERTS_NAMESPACE::FJsEnv> JsEnv;
};