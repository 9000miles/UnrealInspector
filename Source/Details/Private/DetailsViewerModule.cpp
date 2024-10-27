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
#include "Creater/NumericCreater.h"
#include "Creater/Vector2Creater.h"
#include "Creater/Vector4Creater.h"
#include "Creater/Vector3Creater.h"
#include "Creater/GuidCreater.h"
#include "Creater/TransformCreater.h"

#define LOCTEXT_NAMESPACE "FPropertyInspectorModule"

using namespace DETAILS_VIEWER;

void FDetailsViewerModule::StartupModule()
{
	if (IsRunningCommandlet()) return;

	Factory::Register<FWidgetCreaterBool>();
	Factory::Register<FWidgetCreaterString>();
	Factory::Register<FWidgetCreaterNumeric>();
	Factory::Register<FWidgetCreaterVector2>();
	Factory::Register<FWidgetCreaterVector>();
	Factory::Register<FWidgetCreaterVector4>();
	Factory::Register<FWidgetCreaterTransform>();
	Factory::Register<FWidgetCreaterGuid>();

	Factory::Register<FDetailMaker>();
	Factory::Register<FCustomDetailMaker>();

	Factory::Register<FUObjectDetailHolder>();
	Factory::Register<FUObjectDetailCommander>();

	//FDetailFactory::Get().RegisterCreater(Factory::Get<IDetailWidgetCreater>(FWidgetCreaterBool::TypeName()).ToSharedRef());
	//FDetailFactory::Get().RegisterCreater(Factory::Get<IDetailWidgetCreater>(FWidgetCreaterString::TypeName()).ToSharedRef());

	//DETAILS_VIEWER_TEST::FDetailsViewerTestModule::RunTest();
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