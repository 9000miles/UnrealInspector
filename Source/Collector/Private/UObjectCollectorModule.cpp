// Fill out your copyright notice in the Description page of Project Settings.
#include "UObjectCollectorModule.h"
#include "UObjectListener.h"

#define LOCTEXT_NAMESPACE "FUObjectCollectorModule"

void FUObjectCollectorModule::StartupModule()
{
	ObjectListener = MakeShared<UObjectCollector::FUObjectListener>();
}

void FUObjectCollectorModule::ShutdownModule()
{
	ObjectListener.Reset();
	ObjectListener = nullptr;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUObjectCollectorModule, UObjectCollector);
