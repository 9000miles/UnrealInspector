// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SDetailView.h"
#include "SlateOptMacros.h"
#include "Core/DetailDefine.h"
#include "View/SDetailTreeItem.h"
#include "View/SDetailCategoryHeader.h"
#include "View/SDetailPropertyWidget.h"
#include "Core/DetailObjectWidget.h"
#include "Core/SWidgetHandle.h"
#include "HAL/UnrealMemory.h"
#include "Core/DetailFactory.h"
#include "Node/MapNode.h"
#include "Node/ArrayNode.h"
#include "Node/SetNode.h"
#include "Node/StructNode.h"
#include "Node/GeneralNode.h"

namespace DETAILS_VIEWER
{

	void SDetailView::Construct(const FArguments& InArgs)
	{
		DetailInfo = InArgs._DetailInfo;

		GenerateTreeNodes();

		TreeView = SNew(STreeView<TSharedPtr<FTreeNode>>)
			.TreeItemsSource(&TreeNodes)
			.OnGenerateRow(this, &SDetailView::OnGenerateRow)
			.OnGetChildren(this, &SDetailView::OnGetChildren)
			.SelectionMode(ESelectionMode::SingleToggle)
			;

		DetailContentBoxPtr = SNew(SBox)
			;

		ChildSlot
			[
				SNew(SBox)
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							[
								SNew(STextBlock).Text(FText::FromString(DetailInfo->Name))
							]
							+ SVerticalBox::Slot()
							[
								TreeView.ToSharedRef()
							]
					]
			];

	}

	void SDetailView::InitByOptions(FDetailOptions Options)
	{

	}

	//void SDetailView::SetObject(UObject* Object)
	//{
	//	if (Object == nullptr)
	//	{
	//		return;
	//	}

	//	TreeNodes.Empty();

	//	UClass* Class = Object->GetClass();
	//	const FString CustomDetail = Class->GetMetaData(TEXT("CustomDetail"));
	//	if (CustomDetail.IsEmpty())
	//	{

	//		for (TFieldIterator<UE_Property> PropertyIt(Class, EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
	//		{
	//			auto Property = *PropertyIt;
	//			const FString PropertyName = Property->GetName();
	//			const FString PropertyType = Property->GetCPPType();
	//			const TSharedPtr<IDetailWidgetCreater> Builder = FDetailFactory::Get().FindCreater(PropertyType);
	//			UE_LOG(LogTemp, Log, TEXT("%s: %s"), *PropertyName, *PropertyType);
	//			//if (Builder == nullptr) continue;

	//			UE_LOG(LogTemp, Log, TEXT("Builder> %s: %s"), *PropertyName, *PropertyType);

	//			TSharedPtr<FTreeNode> Node = GenerateNode(Object, Property);
	//			if (Node == nullptr)
	//			{
	//				UE_LOG(LogRuntimeDetail, Warning, TEXT("%s %s property generate node failed"), *Object->GetName(), *Property->GetName());
	//				continue;
	//			}

	//			TreeNodes.Add(Node);
	//		}

	//		TreeView->RequestTreeRefresh();
	//		DetailContentBoxPtr->SetContent(TreeView.ToSharedRef());
	//	}
	//	else
	//	{
	//	}
	//}

	TSharedRef<class ITableRow> SDetailView::OnGenerateRow(TSharedPtr<FTreeNode> Node, const TSharedRef<class STableViewBase>& OwnerTable)
	{
		TSharedPtr<SWidget> RowWidget;

		if (Node->GetTypeName() == FCategoryTreeNode::TypeName())
		{
			RowWidget = SNew(SDetailCategoryHeader);
		}
		else
		{
			FString OutCustomType;
			bool bOverrideRowWidget = false;
			//TSharedPtr<SWidget> CustomRowWidget = CreateCustomDetailRowWidget(Node, OutCustomType, bOverrideRowWidget);

			//if (CustomRowWidget.IsValid())
			//{
			//	RowWidget = CustomRowWidget;
			//}
			//else
			//{
			TSharedPtr<SDetailPropertyWidget> PropertyWidget = SNew(SDetailPropertyWidget, Node, bOverrideRowWidget, nullptr)
				.OnSplitterSlotResized(this, &SDetailView::OnSplitterSlotResized)
				;

			RowWidget = PropertyWidget;
			HasSplitterWidgets.Add(PropertyWidget);
			//}
		}

		return SNew(STableRow<TSharedPtr<FTreeNode>>, OwnerTable)
			[
				RowWidget.ToSharedRef()
			]
			;
	}


	//TSharedPtr<SWidget> SDetailView::CreateCustomDetailRowWidget(TSharedPtr<FTreeNode> Node, FString& OutCustomType, bool& bOutOverrideRowWidget)
	//{
	//	TSharedPtr<FPropertyHolder> Proxy = Node->GetPropertyHolder();
	//	UE_Property* Property = Proxy->GetProperty();
	//	const FString CustomPropertyBuilderPath = Property->GetMetaData(TEXT("PropertyBuilder"));
	//	if (!CustomPropertyBuilderPath.IsEmpty())
	//	{
	//		//UClass* CustomPropertyBuilderClass = LoadClass<FPropertyWidgetCreater>(nullptr, *CustomPropertyBuilderPath);
	//		//check(CustomPropertyBuilderClass != nullptr);

	//		//TStrongObjectPtr<FPropertyWidgetCreater> CustomPropertyBuilder = TStrongObjectPtr<FPropertyWidgetCreater>(NewObject<FPropertyWidgetCreater>(GetTransientPackage(), CustomPropertyBuilderClass));
	//		//bOutOverrideRowWidget = CustomPropertyBuilder->OverrideRowWidget();

	//		//CutomPropertyBuilders.Add(CustomPropertyBuilder);
	//		//return CustomPropertyBuilder->CreateWidget(Proxy);
	//		return SNullWidget::NullWidget;
	//	}

	//	//const FString CustomFuncRowWidget = FString();
	//	const FString CustomFuncRowWidget = Property->GetMetaData(TEXT("FuncPropertyRowWidget"));
	//	if (!CustomFuncRowWidget.IsEmpty())
	//	{
	//		TWeakObjectPtr<UObject> Object = Node->GetObject();
	//		UFunction* Function = Object->FindFunction(*CustomFuncRowWidget);
	//		check(Function != nullptr);

	//		struct
	//		{
	//			FPropertyHandle Handle;
	//		}ParamsAddress;
	//		ParamsAddress.Handle.Property = Property;

	//		Object->ProcessEvent(Function, &ParamsAddress);

	//		if (Function->ReturnValueOffset != MAX_uint16)
	//		{
	//			void* Value = &ParamsAddress + 1;
	//			return *(TSharedPtr<SWidget>*)Value;
	//		}

	//		return nullptr;
	//	}

	//	return nullptr;
	//}

	void SDetailView::OnGetChildren(TSharedPtr<FTreeNode> Node, TArray<TSharedPtr<FTreeNode>>& Children)
	{
		Children = Node->GetChildren();
	}


	void SDetailView::GenerateTreeNodes()
	{
		DetailInfo->CategoryList->Enumerate([this](TSharedPtr<FCategoryInfo> CategoryInfo)
			{
				TSharedPtr<FCategoryTreeNode> CategoryNode = MakeShareable(new FCategoryTreeNode(CategoryInfo));
				TreeNodes.Add(CategoryNode);

				CategoryInfo->PropertyList->Enumerate([CategoryNode](TSharedPtr<FPropertyInfo> PropertyInfo)
					{
						TSharedPtr<FPropertyTreeNode> Node = MakeShareable(new FPropertyTreeNode(PropertyInfo));
						CategoryNode->AddChild(Node);
					});

			});
	}

	//TSharedPtr<FTreeNode> SDetailView::GenerateNode(UObject* Object, UE_Property* Property)
	//{
	//	if (Property->IsA(FArrayProperty::StaticClass()))
	//		return MakeShareable(new FArrayNode(Object, Property));
	//	else if (Property->IsA(FMapProperty::StaticClass()))
	//		return MakeShareable(new FMapNode(Object, Property));
	//	else if (Property->IsA(FSetProperty::StaticClass()))
	//		return MakeShareable(new FSetNode(Object, Property));
	//	else if (Property->IsA(FStructProperty::StaticClass()))
	//		return MakeShareable(new FStructNode(Object, Property));

	//	return MakeShareable(new FNormalNode(Object, Property));
	//}

	void SDetailView::OnSplitterSlotResized(int32 Index, float Size)
	{
		for (TSharedPtr<SDetailPropertyWidget> DetialItem : HasSplitterWidgets)
		{
			if (!DetialItem->HasSplitter()) continue;

			DetialItem->SetSplitterSlotSize(Index, Size);
		}
	}

}