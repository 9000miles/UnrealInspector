// Copyright Epic Games, Inc. All Rights Reserved.

#include "Inspector.h"
#include "UObjectInfo.h"
#include "Core/DetialManger.h"
#include "Core/DetailDefine.h"
#include "Detail/UObjectDetail.h"
#include "UObjectCollector.h"
#include "Widgets/Input/SSearchBox.h"
#include "UObjectHolder.h"
#include "Widgets/Views/STileView.h"

#define LOCTEXT_NAMESPACE "FInspectorModule"

void FInspectorModule::StartupModule()
{
	FUObjectCollector::OnObjectAddEvent.AddRaw(this, &FInspectorModule::OnObjectAdded);
	FUObjectCollector::OnObjectDeleteEvent.AddRaw(this, &FInspectorModule::OnObjectDeleted);

	FUObjectCollector::Get().GetAll(ObjectList);

	ObjectTreeView = SNew(STreeView<TSharedPtr<FUObjectHolder>>)
		.OnGetChildren_Raw(this, &FInspectorModule::OnGetChildren)
		.TreeItemsSource(&ObjectList)
		.OnGenerateRow_Raw(this, &FInspectorModule::GenerateRowWidget)
		.OnSelectionChanged_Lambda([this](TSharedPtr<FUObjectHolder> Item, ESelectInfo::Type SelectInfo)
			{
				DetailHolder->SetObject(Item->Get());
			})
		;

	ClassifyList.Add(MakeShared<FUObjectClassify>(EClassifyType::Class));
	ClassifyList.Add(MakeShared<FUObjectClassify>(EClassifyType::Outer));
	ClassifyList.Add(MakeShared<FUObjectClassify>(EClassifyType::Package));

	ObjectInspector = SNew(SBox)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STileView<TSharedPtr<FUObjectClassify>>)
						.ListItemsSource(&ClassifyList)
						.SelectionMode(ESelectionMode::SingleToggle)
						.ItemHeight(40)
						.ItemWidth(80)
						.ClearSelectionOnClick(false)
						.OnSelectionChanged_Raw(this, &FInspectorModule::OnClassifySelectionChanged)
						.OnGenerateTile_Lambda([this](TSharedPtr<FUObjectClassify> Item, const TSharedRef< class STableViewBase >& OwnerTable)
							{
								return SNew(STableRow<TSharedPtr<FUObjectClassify>>, OwnerTable)
									[
										SNew(SBox)
											.VAlign(VAlign_Center)
											.HAlign(HAlign_Center)
											[
												SNew(STextBlock).Text(FText::FromString(Item->GetTypeName()))
											]
									]
									;
							})
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SSearchBox)
						.OnTextChanged_Raw(this, &FInspectorModule::OnSearchTextChanged)
				]
				+ SVerticalBox::Slot()
				[
					ObjectTreeView.ToSharedRef()
				]
		]
		;

	TSharedPtr<SWindow> Window = SNew(SWindow)
		.Title(LOCTEXT("InspectorTitle", "Unreal Inspector"))
		.ClientSize(FVector2D(1600, 1200))
		.MinWidth(400)
		.MinHeight(300)
		.Content()
		[
			MakeWidget()
		]
		;

	FSlateApplication::Get().AddWindow(Window.ToSharedRef());
}

TSharedRef<class ITableRow> FInspectorModule::GenerateRowWidget(TSharedPtr<FUObjectHolder> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FUObjectHolder>>, OwnerTable)
		.Visibility_Raw(this, &FInspectorModule::GetRowVisible, Item)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(2.0f)
				[
					SNew(STextBlock)
						.Text(FText::FromString(Item->GetName()))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.Padding(2.0f)
				[
					SNew(STextBlock)
						.Text(FText::FromString(Item->GetClassName()))
				]
		];
}

TSharedRef<SWidget> FInspectorModule::MakeWidget()
{
	FDetialManager& Manager = FDetialManager::Get();

	TSharedPtr<FDetailOptions> Options = MakeShared<FDetailOptions>();
	DetailHolder = Manager.Create<FUObjectDetailHolder>(Options);
	TSharedPtr<SWidget> DetailViewer = DetailHolder->GetWidget();

	TSharedPtr<SWidget> Widget =
		SNew(SSplitter)
		+ SSplitter::Slot()
		.Value(0.35f)
		[
			ObjectInspector.ToSharedRef()
		]
		+ SSplitter::Slot()
		[
			DetailViewer.ToSharedRef()
		]
		;

	return Widget.ToSharedRef();
}

void FInspectorModule::ShutdownModule()
{
}

void FInspectorModule::OnObjectAdded(TSharedPtr<FUObjectHolder> ObjectInfo)
{
	ObjectList.Add(ObjectInfo);
	ObjectTreeView->RequestListRefresh();
}
void FInspectorModule::OnObjectDeleted(TSharedPtr<FUObjectHolder> ObjectInfo)
{
	ObjectList.Remove(ObjectInfo);
	ObjectTreeView->RequestListRefresh();
}

void FInspectorModule::OnSearchTextChanged(const FText& Text)
{
	const bool bIsEmpty = Text.IsEmpty();

	for (const auto Holder : ObjectList)
	{
		if (bIsEmpty)
			Holder->CloseSearch();
		else
			Holder->OnSearch(Text);
	}
}

EVisibility FInspectorModule::GetRowVisible(TSharedPtr<FUObjectHolder> Holder) const
{
	if (Holder->IsValid())
		return Holder->IsVisible() ? EVisibility::Visible : EVisibility::Collapsed;

	return EVisibility::Collapsed;
}

void FInspectorModule::OnClassifySelectionChanged(TSharedPtr<FUObjectClassify> NewSelection, ESelectInfo::Type SelectInfo)
{
	if (!NewSelection.IsValid()) return;

	UE_LOG(LogTemp, Warning, TEXT("OnClassifySelectionChanged %d"), NewSelection->ClassifyType);
}

void FInspectorModule::OnGetChildren(TSharedPtr<FUObjectHolder> Node, TArray<TSharedPtr<FUObjectHolder>>& Children)
{
	Children = Node->GetChildren();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInspectorModule, Inspector)