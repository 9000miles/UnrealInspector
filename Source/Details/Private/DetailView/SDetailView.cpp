// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailView/SDetailView.h"
#include "SlateOptMacros.h"
#include "DetailCore/DetailDefine.h"
#include "DetailView/SDetailTreeItem.h"
#include "DetailView/SDetailCategoryHeader.h"
#include "DetailView/SDetailPropertyWidget.h"
#include "DetailCore/DetailObjectWidget.h"
#include "DetailCore/SWidgetHandle.h"
#include "HAL/UnrealMemory.h"
#include "DetailCore/DetailFactory.h"

namespace DetailsViewer
{

	void SDetailView::Construct(const FArguments& InArgs)
	{
		TreeView = SNew(STreeView<TSharedPtr<FDetailTreeNode>>)
			.TreeItemsSource(&PropertyNodes)
			.OnGenerateRow(this, &SDetailView::OnGenerateRow)
			.OnGetChildren(this, &SDetailView::OnGetChildren)
			.SelectionMode(ESelectionMode::SingleToggle)
			;

		DetailContentBoxPtr = SNew(SBox)
			;

		ChildSlot
			[
				DetailContentBoxPtr.ToSharedRef()
			];

	}

	void SDetailView::InitByOptions(FDetailOptions Options)
	{

	}

	void SDetailView::SetObject(UObject* Object)
	{
		if (Object == nullptr)
		{
			return;
		}

		PropertyNodes.Empty();

		UClass* Class = Object->GetClass();
		const FString CustomDetail = Class->GetMetaData(TEXT("CustomDetail"));
		if (CustomDetail.IsEmpty())
		{

			for (TFieldIterator<UE_Property> PropertyIt(Class, EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
			{
				auto Property = *PropertyIt;
				const FString PropertyName = Property->GetName();
				const FString PropertyType = Property->GetCPPType();
				const TSharedPtr<IDetailWidgetCreater> Builder = FDetailFactory::Get().FindCreater(PropertyType);
				UE_LOG(LogTemp, Log, TEXT("%s: %s"), *PropertyName, *PropertyType);
				//if (Builder == nullptr) continue;

				UE_LOG(LogTemp, Log, TEXT("Builder> %s: %s"), *PropertyName, *PropertyType);

				TSharedPtr<FDetailTreeNode> Node = GenerateNode(Object, Property);
				if (Node == nullptr)
				{
					UE_LOG(LogRuntimeDetail, Warning, TEXT("%s %s property generate node failed"), *Object->GetName(), *Property->GetName());
					continue;
				}

				PropertyNodes.Add(Node);
			}

			TreeView->RequestTreeRefresh();
			DetailContentBoxPtr->SetContent(TreeView.ToSharedRef());
		}
		else
		{
		}
	}

	TSharedRef<class ITableRow> SDetailView::OnGenerateRow(TSharedPtr<FDetailTreeNode> Node, const TSharedRef<class STableViewBase>& OwnerTable)
	{
		TSharedPtr<SWidget> RowWidget;

		if (Node)
		{
			RowWidget = SNew(SDetailCategoryHeader);
		}
		else
		{
			FString OutCustomType;
			bool bOverrideRowWidget = false;
			TSharedPtr<SWidget> CustomRowWidget = CreateCustomDetailRowWidget(Node, OutCustomType, bOverrideRowWidget);

			//if (CustomRowWidget.IsValid())
			//{
			//	RowWidget = CustomRowWidget;
			//}
			//else
			//{
			TSharedPtr<SDetailPropertyWidget> PropertyWidget = SNew(SDetailPropertyWidget, Node, bOverrideRowWidget, CustomRowWidget)
				.OnSplitterSlotResized(this, &SDetailView::OnSplitterSlotResized)
				;

			RowWidget = PropertyWidget;
			HasSplitterWidgets.Add(PropertyWidget);
			//}
		}

		return SNew(STableRow<TSharedPtr<FDetailTreeNode>>, OwnerTable)
			[
				RowWidget.ToSharedRef()
			]
			;
	}


	TSharedPtr<SWidget> SDetailView::CreateCustomDetailRowWidget(TSharedPtr<FDetailTreeNode> Node, FString& OutCustomType, bool& bOutOverrideRowWidget)
	{
		TSharedPtr<FPropertyHolder> Proxy = Node->GetPropertyHolder();
		UE_Property* Property = Proxy->GetProperty();
		const FString CustomPropertyBuilderPath = Property->GetMetaData(TEXT("PropertyBuilder"));
		if (!CustomPropertyBuilderPath.IsEmpty())
		{
			//UClass* CustomPropertyBuilderClass = LoadClass<FPropertyWidgetCreater>(nullptr, *CustomPropertyBuilderPath);
			//check(CustomPropertyBuilderClass != nullptr);

			//TStrongObjectPtr<FPropertyWidgetCreater> CustomPropertyBuilder = TStrongObjectPtr<FPropertyWidgetCreater>(NewObject<FPropertyWidgetCreater>(GetTransientPackage(), CustomPropertyBuilderClass));
			//bOutOverrideRowWidget = CustomPropertyBuilder->OverrideRowWidget();

			//CutomPropertyBuilders.Add(CustomPropertyBuilder);
			//return CustomPropertyBuilder->CreateWidget(Proxy);
			return SNullWidget::NullWidget;
		}

		//const FString CustomFuncRowWidget = FString();
		const FString CustomFuncRowWidget = Property->GetMetaData(TEXT("FuncPropertyRowWidget"));
		if (!CustomFuncRowWidget.IsEmpty())
		{
			TWeakObjectPtr<UObject> Object = Node->GetObject();
			UFunction* Function = Object->FindFunction(*CustomFuncRowWidget);
			check(Function != nullptr);

			struct
			{
				FPropertyHandle Handle;
			}ParamsAddress;
			ParamsAddress.Handle.Property = Property;

			Object->ProcessEvent(Function, &ParamsAddress);

			if (Function->ReturnValueOffset != MAX_uint16)
			{
				void* Value = &ParamsAddress + 1;
				return *(TSharedPtr<SWidget>*)Value;
			}

			return nullptr;
		}

		return nullptr;
	}

	void SDetailView::OnGetChildren(TSharedPtr<FDetailTreeNode> Node, TArray<TSharedPtr<FDetailTreeNode>>& Children)
	{
		Children = Node->GetChildNodes();
	}

	TSharedPtr<FDetailTreeNode> SDetailView::GenerateNode(UObject* Object, UE_Property* Property)
	{
		TSharedPtr<FDetailTreeNode> Node = MakeShareable(new FDetailTreeNode(EDetailNodeType::NormalProperty, Object, Property));
		return Node;
	}

	void SDetailView::OnSplitterSlotResized(int32 Index, float Size)
	{
		for (TSharedPtr<SDetailPropertyWidget> DetialItem : HasSplitterWidgets)
		{
			if (!DetialItem->HasSplitter()) continue;

			DetialItem->SetSplitterSlotSize(Index, Size);
		}
	}

}