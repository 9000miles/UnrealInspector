// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inspector.h"
#include "UObjectInfo.h"
#include "Core/DetialManger.h"
#include "Core/DetailDefine.h"
#include "Detail/UObjectDetail.h"
#include "UObjectCollector.h"

#define LOCTEXT_NAMESPACE "FInspectorModule"

void FInspectorModule::StartupModule()
{
	//FUObjectCollector::OnObjectAddEvent.AddRaw(this, &FInspectorModule::OnObjectAdded);
	//FUObjectCollector::OnObjectDeleteEvent.AddRaw(this, &FInspectorModule::OnObjectDeleted);

	FDetialManager& Manager = FDetialManager::Get();

	TSharedPtr<FDetailOptions> Options = MakeShared<FDetailOptions>();
	DetailHolder = Manager.Create<FUObjectDetailHolder>(Options);
	TSharedPtr<SWidget> DetailViewer = DetailHolder->GetWidget();

	FUObjectCollector::Get().GetAll(ObjectList);

	ObjectListView = SNew(SListView<TSharedPtr<FUObjectHolder>>)
		.ListItemsSource(&ObjectList)
		.OnGenerateRow_Lambda([this](TSharedPtr<FUObjectHolder> Item, const TSharedRef<STableViewBase>& OwnerTable)
			{
				return SNew(STableRow<TSharedPtr<FUObjectHolder>>, OwnerTable)
					[
						SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							.Padding(2.0f)
							[
								SNew(STextBlock)
									.Text(FText::FromString(Item->GetObjectInfo()->GetName()))
							]
							+ SHorizontalBox::Slot()
							.HAlign(HAlign_Right)
							.Padding(2.0f)
							[
								SNew(STextBlock)
									.Text(FText::FromString(Item->GetObjectInfo()->GetClassName()))
							]
					];
			})
		.OnSelectionChanged_Lambda([this](TSharedPtr<FUObjectHolder> Item, ESelectInfo::Type SelectInfo)
			{
				DetailHolder->SetObject(Item->Get());
			})
		;

	TSharedPtr<SWindow> Window = SNew(SWindow)
		.Title(LOCTEXT("InspectorTitle", "Unreal Inspector"))
		.ClientSize(FVector2D(1600, 1200))
		.MinWidth(400)
		.MinHeight(300)
		.Content()
		[
			SNew(SSplitter)
				+ SSplitter::Slot()
                .Value(0.35f)
				[
					ObjectListView.ToSharedRef()
				]
				+ SSplitter::Slot()
				[
					DetailViewer.ToSharedRef()
				]
		]
		;

	FSlateApplication::Get().AddWindow(Window.ToSharedRef());
}

void FInspectorModule::ShutdownModule()
{
}

void FInspectorModule::OnObjectAdded(TSharedPtr<FUObjectHolder> ObjectInfo)
{


}
void FInspectorModule::OnObjectDeleted(TSharedPtr<FUObjectHolder> ObjectInfo)
{


}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInspectorModule, Inspector)