// Copyright Epic Games, Inc. All Rights Reserved.

#include "DetailsViewerModule.h"
#include "DetailCore/DetialManger.h"
#include "DetailCore/DetailViewer.h"
#include "DetailTest/DetailTestObject.h"
#include "DetailCore/DetailFactory.h"
#include "DetailCreater/CreaterBool.h"
#include "DetailCreater/CreaterString.h"
#include "DetailCore/DetailInfo.h"

#define LOCTEXT_NAMESPACE "FPropertyInspectorModule"

using namespace DetailsViewer;

void FDetailsViewerModule::StartupModule()
{
	if (IsRunningCommandlet()) return;

	Factory::Register<FWidgetCreaterBool>();
	Factory::Register<FWidgetCreaterString>();

	Factory::Register<FDetailMaker>();
	Factory::Register<FCustomDetailMaker>();

	DetailsViewerTest::FDetailsViewerTestModule::RunTest();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

template<typename T>
void FDetailsViewerModule::RegisterCreater()
{
	TSharedPtr<T> Creater = MakeShared<T>();
	DetailsViewer::FDetailFactory::Get().RegisterCreater(Creater.ToSharedRef());
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