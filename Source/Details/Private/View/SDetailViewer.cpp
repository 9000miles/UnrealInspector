// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SDetailViewer.h"
#include "SlateOptMacros.h"
#include "Core/DetailDefine.h"
#include "View/SDetailTreeItem.h"
#include "View/SDetailCategory.h"
#include "View/SDetailProperty.h"
#include "Core/DetailObjectWidget.h"
#include "Core/SWidgetHandle.h"
#include "HAL/UnrealMemory.h"
#include "Core/DetailFactory.h"
#include "Node/MapNode.h"
#include "Node/ArrayNode.h"
#include "Node/SetNode.h"
#include "Node/StructNode.h"
#include "Node/GeneralNode.h"
#include "Widgets/Input/SSearchBox.h"

namespace DETAILS_VIEWER
{
#define LOCTEXT_NAMESPACE "SDetailViewer"
	void SDetailViewer::Construct(const FArguments& InArgs)
	{
		DetailInfo = InArgs._DetailInfo;

		GenerateTreeNodes();

		TreeView = SNew(STreeView<TSharedPtr<FTreeNode>>)
			.TreeItemsSource(&TreeNodes)
			.OnGenerateRow(this, &SDetailViewer::OnGenerateRow)
			.OnGetChildren(this, &SDetailViewer::OnGetChildren)
			.SelectionMode(ESelectionMode::SingleToggle)
			.OnContextMenuOpening(this, &SDetailViewer::OnContextMenuOpening)
			;

		DetailContentBoxPtr = SNew(SBox)
			;
		// 显示名称
		NameWidget = SNew(STextBlock)
			.Text(FText::FromString(DetailInfo.IsValid() ? DetailInfo->Name : TEXT("")))
			;

		ChildSlot
			[
				SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("DetailsView.CategoryTop"))
					.Padding(FMargin(3.0f))
					[
						SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								NameWidget.ToSharedRef()
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SNew(SSearchBox)
									.OnTextChanged(this, &SDetailViewer::OnSearchTextChanged)
									.OnTextCommitted(this, &SDetailViewer::OnSearchTextCommitted)
							]
							+ SVerticalBox::Slot()
							//.FillHeight()
							[
								// 显示属性树
								TreeView.ToSharedRef()
							]
					]
			];


	}

	void SDetailViewer::SetDetailInfo(TSharedPtr<FDetailInfo> InDetailInfo)
	{
		DetailInfo = InDetailInfo;
		if (DetailInfo.IsValid())
		{
			NameWidget->SetText(FText::FromString(DetailInfo->Name));
		}
		else
		{
			NameWidget->SetText(FText::FromString(""));
		}

		GenerateTreeNodes();
		TreeView->RequestTreeRefresh();
	}

	void SDetailViewer::InitByOptions(FDetailOptions Options)
	{

	}

	TSharedPtr<SWidget> SDetailViewer::OnContextMenuOpening()
	{
		TSharedPtr< const FUICommandList > CommandList;

		TArray<TSharedPtr<FTreeNode>> SelectedItems;
		const int32 SelectedCount = TreeView->GetSelectedItems(SelectedItems);
		if (SelectedCount <= 0) return SNullWidget::NullWidget;

		TSharedPtr<FTreeNode> SelectedItem = SelectedItems[0];
		FMenuBuilder MenuBuilder(true, CommandList);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("Copy", "Copy"),
			LOCTEXT("CopyTooltip", "Copy the value to the clipboard"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &SDetailViewer::Copy, SelectedItem))
		);

		MenuBuilder.AddMenuEntry(
			LOCTEXT("Paste", "Paste"),
			LOCTEXT("PasteTooltip", "Paste the value from the clipboard"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateSP(this, &SDetailViewer::Paste, SelectedItem))
		);

		return MenuBuilder.MakeWidget();
	}

	void SDetailViewer::Copy(TSharedPtr<FTreeNode> Node)
	{
		Node->Copy();

		UE_LOG(LogTemp, Warning, TEXT("Copy the value to the clipboard"));
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

	TSharedRef<class ITableRow> SDetailViewer::OnGenerateRow(TSharedPtr<FTreeNode> Node, const TSharedRef<class STableViewBase>& OwnerTable)
	{
		TSharedPtr<SDetailTreeItem> RowWidget;

		if (Node->GetTypeName() == FCategoryTreeNode::TypeName())
		{
			//TSharedPtr<FCategoryTreeNode> CategoryNode = StaticCastSharedPtr<FCategoryTreeNode>(Node);
			RowWidget = SNew(SDetailCategory, Node);
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
			//TSharedPtr<FPropertyTreeNode> PropertyNode = StaticCastSharedPtr<FPropertyTreeNode>(Node);
			TSharedPtr<SDetailProperty> PropertyWidget = SNew(SDetailProperty, Node, bOverrideRowWidget, nullptr)
				.OnSplitterSlotResized(this, &SDetailViewer::OnSplitterSlotResized)
				;

			RowWidget = PropertyWidget;
			//}
		}

		if (RowWidget->HasSplitter())
			HasSplitterWidgets.Add(RowWidget);

		return SNew(STableRow<TSharedPtr<FTreeNode>>, OwnerTable)
			[
				RowWidget.ToSharedRef()
			]
			.Visibility(this, &SDetailViewer::IsCanVisible, Node)
			;
	}


	void SDetailViewer::OnSearchTextChanged(const FText& Text)
	{
		for (TSharedPtr<FTreeNode> Node : TreeNodes)
		{
			Node->OnSearch(Text);
		}
	}

	void SDetailViewer::Paste(TSharedPtr<FTreeNode> Node)
	{
		Node->Paste();
		UE_LOG(LogTemp, Warning, TEXT("Paste the value from the clipboard"));
	}

	EVisibility SDetailViewer::IsCanVisible(TSharedPtr<FTreeNode> Node) const
	{
		const bool bVisible = Node->IsCanVisible();
		return bVisible ? EVisibility::Visible : EVisibility::Collapsed;
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

	void SDetailViewer::OnGetChildren(TSharedPtr<FTreeNode> Node, TArray<TSharedPtr<FTreeNode>>& Children)
	{
		Children = Node->GetChildren();
	}


	void SDetailViewer::GenerateTreeNodes()
	{
		if (!DetailInfo.IsValid()) return;

		// 清空现有节点
		TreeNodes.Empty();

		// 遍历所有分类信息
		DetailInfo->CategoryList->Enumerate([this](TSharedPtr<FCategoryInfo> CategoryInfo)
			{
				// 创建分类节点
				TSharedPtr<FCategoryTreeNode> CategoryNode = MakeShareable(new FCategoryTreeNode(CategoryInfo));
				TreeNodes.Add(CategoryNode);

				// 遍历分类中的所有属性信息
				CategoryInfo->PropertyList->Enumerate([CategoryNode](TSharedPtr<FPropertyInfo> PropertyInfo)
					{
						PropertyInfo->MakeTreeNode(CategoryNode);
					});
			});

		// 这里可以添加额外的逻辑，例如排序或过滤树节点等
	}


	void SDetailViewer::OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitType)
	{

	}

	void SDetailViewer::OnSplitterSlotResized(int32 Index, float Size)
	{
		for (TSharedPtr<SDetailTreeItem> DetialItem : HasSplitterWidgets)
		{
			if (!DetialItem->HasSplitter()) continue;
			//Size = Size - (DetialItem->GetNode<FTreeNode>()->GetDepth() * 0.005f);
			//Size = DetialItem->GetSplitterSlotSize();


			DetialItem->SetSplitterSlotSize(Index, Size);
		}
	}
#undef LOCTEXT_NAMESPACE
}