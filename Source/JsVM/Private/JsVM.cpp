// Fill out your copyright notice in the Description page of Project Settings.
#include "JsVM.h"
#include "Engine/World.h"
#include "JSModuleLoader.h"

#define LOCTEXT_NAMESPACE "FJsVMModule"

void FJsVMModule::StartupModule()
{
	//#if WITH_EDITOR
	FWorldDelegates::OnStartGameInstance.AddRaw(this, &FJsVMModule::OnStartGameInstance);
	//#endif
}

void FJsVMModule::ShutdownModule()
{
}

void FJsVMModule::OnStartGameInstance(UGameInstance* GameInstance)
{
	JsEnv = MakeShared<PUERTS_NAMESPACE::FJsEnv>(
		std::make_shared<PUERTS_NAMESPACE::DefaultJSModuleLoader>(TEXT("JavaScript")),
		std::make_shared<PUERTS_NAMESPACE::FDefaultLogger>(),
		9856
	);

    //JsEnv->Start(TEXT("Scripts/Start/StartScript"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FJsVMModule, JsVM);
