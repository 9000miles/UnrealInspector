// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/DetailDefine.h"
#include "Core/DetailFactory.h"
#include "Widgets/Views/STreeView.h"
#include "Node/DetailNode.h"
#include "SDetailTreeItem.h"
#include "SDetailProperty.h"
#include "Core/DetailInfo.h"


namespace DETAILS_VIEWER
{

	/**
	 *
	 */
	class DETAILSVIEWER_API SDetailViewer : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SDetailViewer)
			{}
			SLATE_ARGUMENT(FDetailOptions, Options)
			SLATE_ARGUMENT(TSharedPtr<FDetailInfo>, DetailInfo)
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs);

		void InitByOptions(FDetailOptions Options);
		//void SetObject(UObject* Object);

	private:
		void GenerateTreeNodes();
		TSharedRef<class ITableRow> OnGenerateRow(TSharedPtr<FTreeNode> Node, const TSharedRef<class STableViewBase>& TableView);
		//TSharedPtr<SWidget> CreateCustomDetailRowWidget(TSharedPtr<FTreeNode> Node, FString& OutCustomType, bool& bOutOverrideRowWidget);
		void OnGetChildren(TSharedPtr<FTreeNode> Node, TArray<TSharedPtr<FTreeNode>>& Children);

	private:
		//TSharedPtr<FTreeNode> GenerateNode(UObject* Object, UE_Property* Property);
		void OnSplitterSlotResized(int32 Index, float Size);

	private:
		TSharedPtr<FDetailInfo> DetailInfo;

		TSharedPtr<SBox> DetailContentBoxPtr;
		TSharedPtr<STreeView<TSharedPtr<FTreeNode>>> TreeView;
		TArray<TSharedPtr<FTreeNode>> TreeNodes;

		TArray<TSharedPtr<SDetailTreeItem>> HasSplitterWidgets;
	};

}