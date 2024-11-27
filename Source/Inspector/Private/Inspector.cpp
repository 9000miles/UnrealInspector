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
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "SFunctionViewer.h"

#define LOCTEXT_NAMESPACE "FInspectorModule"

using namespace DETAILS_VIEWER;

void FInspectorModule::StartupModule()
{

	Factory::Register<FFunctionDetailHolder>();
	//�ֶ�ˢ��
	//FUObjectCollector::OnObjectAddEvent.AddRaw(this, &FInspectorModule::OnObjectAdded);
	//FUObjectCollector::OnObjectDeleteEvent.AddRaw(this, &FInspectorModule::OnObjectDeleted);

	FUObjectCollector::Get().GetAll(ObjectList);

	ObjectTreeView = SNew(STreeView<TSharedPtr<FUObjectHolder>>)
		.OnGetChildren_Raw(this, &FInspectorModule::OnGetChildren)
		.TreeItemsSource(&ObjectList)
		.OnGenerateRow_Raw(this, &FInspectorModule::GenerateRowWidget)
		.OnSelectionChanged_Lambda([this](TSharedPtr<FUObjectHolder> Item, ESelectInfo::Type SelectInfo)
			{
				if (!Item.IsValid()) return;

				DetailHolder->SetObject(Item->Get());
				FunctionsWidget->SetObject(Item->Get());
			})
		;

	SearchTypeList.Add(MakeShared<FObjectSearchType>(EObjectSearchType::Name));
	SearchTypeList.Add(MakeShared<FObjectSearchType>(EObjectSearchType::Path));
	SearchTypeList.Add(MakeShared<FObjectSearchType>(EObjectSearchType::Class));
	SearchTypeList.Add(MakeShared<FObjectSearchType>(EObjectSearchType::Package));
	SearchTypeList.Add(MakeShared<FObjectSearchType>(EObjectSearchType::Module));
	SearchTypeList.Add(MakeShared<FObjectSearchType>(EObjectSearchType::Function));

	SAssignNew(SearchTypeComboBox, SComboBox<TSharedPtr<FObjectSearchType>>)
		.OptionsSource(&SearchTypeList)
		.OnSelectionChanged_Lambda([this](TSharedPtr<FObjectSearchType> NewSelection, ESelectInfo::Type SelectInfo)
			{
			})
		.OnGenerateWidget_Lambda([](TSharedPtr<FObjectSearchType> Item)
			{
				return SNew(STextBlock).Text(Item->GetText());
			})
		.InitiallySelectedItem(SearchTypeList[0])
		//.Content()
		[
			SNew(STextBlock).Text_Lambda([this]()
				{
					return SearchTypeComboBox->GetSelectedItem()->GetText();
				})
		]
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
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(2.0f)
						[
							SearchTypeComboBox.ToSharedRef()
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						.Padding(2.0f)
						[
							SNew(SSearchBox)
								.OnTextChanged_Raw(this, &FInspectorModule::OnSearchTextChanged)
						]
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
		.Value(0.5f)
		[
			ObjectInspector.ToSharedRef()
		]
		+ SSplitter::Slot()
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(SButton).Text(LOCTEXT("Properties", "Properties"))
								.OnClicked_Lambda([this]()
									{
										DetailWidgetPtr->SetActiveWidgetIndex(0);
										return FReply::Handled();
									})
						]
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						[
							SNew(SButton).Text(LOCTEXT("Functions", "Functions"))
								.OnClicked_Lambda([this]()
									{
										DetailWidgetPtr->SetActiveWidgetIndex(1);
										return FReply::Handled();
									})
						]
				]
				+ SVerticalBox::Slot()
				[
					SAssignNew(DetailWidgetPtr, SWidgetSwitcher)
						+ SWidgetSwitcher::Slot()
						[
							DetailViewer.ToSharedRef()
						]
						+ SWidgetSwitcher::Slot()
						[
							//SNew(STextBlock).Text(LOCTEXT("No Object Selected", "Functions"))
							MakeFunctionsTree()
						]
				]
		]
		;

	return Widget.ToSharedRef();
}

void FInspectorModule::ShutdownModule()
{
}

TSharedRef<SWidget> FInspectorModule::MakeFunctionsTree()
{
	return SAssignNew(FunctionsWidget, SFunctionViewer)
		;
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
		{
			Holder->CloseSearch();
		}
		else
		{
			const EObjectSearchType SearchType = SearchTypeComboBox->GetSelectedItem()->SearchType;
			Holder->OnSearch(Text, SearchType);
		}
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

	TArray<TSharedPtr<FUObjectHolder>> ObjectArray;
	FUObjectCollector::Get().GetAll(ObjectArray);

	ObjectTreeView->ClearSelection();
	//ObjectTreeView->ClearItemsSource();

	ObjectList.Empty();
	ObjectList = NewSelection->Classify(ObjectArray);
	//ObjectTreeView->SetItemsSource(&ObjectList);

	ObjectTreeView->RequestTreeRefresh();
}

void FInspectorModule::OnGetChildren(TSharedPtr<FUObjectHolder> Node, TArray<TSharedPtr<FUObjectHolder>>& Children)
{
	Node->GetChildren(Children);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FInspectorModule, Inspector)
