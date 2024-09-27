// Copyright Epic Games, Inc. All Rights Reserved.

#include "DetailsViewerModule.h"
#include "Core/DetialManger.h"
#include "Core/DetailViewer.h"
#include "Test/DetailTestObject.h"
#include "Core/DetailFactory.h"
#include "Creater/BoolCreater.h"
#include "Creater/StringCreater.h"
#include "Core/DetailInfo.h"
#include "Detail/UObjectDetail.h"

#define LOCTEXT_NAMESPACE "FPropertyInspectorModule"

using namespace DETAILS_VIEWER;

void FDetailsViewerModule::StartupModule()
{
	if (IsRunningCommandlet()) return;

	Factory::Register<FWidgetCreaterBool>();
	Factory::Register<FWidgetCreaterString>();

	Factory::Register<FDetailMaker>();
	Factory::Register<FCustomDetailMaker>();

	Factory::Register<FUObjectDetail>();

	DETAILS_VIEWER_TEST::FDetailsViewerTestModule::RunTest();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDetailsViewerModule::ShutdownModule()
{
	//TestObject->ConditionalBeginDestroy();
	//TestObject.Reset();
	//TestObject = nullptr;
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDetailsViewerModule, DetailsViewer)