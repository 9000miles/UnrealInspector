// Copyright Epic Games, Inc. All Rights Reserved.

#include "RuntimeDetailModule.h"
#include "DetailCore/DetialManger.h"
#include "DetailCore/DetailViewer.h"
#include "DetailTest/DetailTestObject.h"

#define LOCTEXT_NAMESPACE "FPropertyInspectorModule"

void FDetailsViewerModule::StartupModule()
{
	//if (IsRunningCommandlet()) return;

	//DetManager = UDetialManager::Get();

	//FDetailOptions Options;
	//UDetailViewer* DetViewer = DetManager->CreateDetail(Options);

	//TestObject = TStrongObjectPtr<UDetailTestObject>(NewObject<UDetailTestObject>(DetViewer));
	//DetViewer->SetObject(TestObject.Get());

	//TSharedPtr<SWindow> WindowPtr = SNew(SWindow)
	//	.ClientSize(FVector2D(800, 600))
	//	.HasCloseButton(true)
	//	.SupportsMaximize(true)
	//	;
	//TSharedPtr<SWidget> ViewWidget = DetViewer->GetViewWidget();
	//WindowPtr->SetContent(ViewWidget.ToSharedRef());

	//FSlateApplication::Get().AddWindow(WindowPtr.ToSharedRef());
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