// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inspector.h"
#include "../ObjectListener.h"

#define LOCTEXT_NAMESPACE "FInspectorModule"

void FInspectorModule::StartupModule()
{
	ObjectListener = MakeShared<Inspector::FObjectListener>();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FInspectorModule::ShutdownModule()
{
	ObjectListener.Reset();
	ObjectListener = nullptr;
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInspectorModule, Inspector)