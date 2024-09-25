// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/DetailDefine.h"
#include "Core/DetailFactory.h"
#include "Widgets/Views/STreeView.h"
#include "Node/DetailNode.h"
#include "SDetailTreeItem.h"
#include "SDetailPropertyWidget.h"


namespace DetailsViewer
{

	/**
	 *
	 */
	class DETAILSVIEWER_API SDetailView : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SDetailView)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs);

		void InitByOptions(FDetailOptions Options);
		void SetObject(UObject* Object);

	private:
		TSharedRef<class ITableRow> OnGenerateRow(TSharedPtr<FDetailTreeNode> Node, const TSharedRef<class STableViewBase>& TableView);
		TSharedPtr<SWidget> CreateCustomDetailRowWidget(TSharedPtr<FDetailTreeNode> Node, FString& OutCustomType, bool& bOutOverrideRowWidget);
		void OnGetChildren(TSharedPtr<FDetailTreeNode> Node, TArray<TSharedPtr<FDetailTreeNode>>& Children);

	private:
		TSharedPtr<FDetailTreeNode> GenerateNode(UObject* Object, UE_Property* Property);
		void OnSplitterSlotResized(int32 Index, float Size);

	private:
		TSharedPtr<SBox> DetailContentBoxPtr;
		TSharedPtr<STreeView<TSharedPtr<FDetailTreeNode>>> TreeView;
		TArray<TSharedPtr<FDetailTreeNode>> PropertyNodes;

		TArray<TSharedPtr<SDetailPropertyWidget>> HasSplitterWidgets;
	};

}