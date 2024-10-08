// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SDetailTreeItem.h"
#include "Widgets/Layout/SSplitter.h"
#include "Node/DetailNode.h"

DECLARE_DELEGATE_TwoParams(FOnPropertySpiltterSlotResized, int32, float);

namespace DETAILS_VIEWER
{

	/**
	 *
	 */
	class DETAILSVIEWER_API SDetailPropertyWidget : public SDetailTreeItem
	{
	public:
		SLATE_BEGIN_ARGS(SDetailPropertyWidget)
			{}
			SLATE_EVENT(FOnPropertySpiltterSlotResized, OnSplitterSlotResized)
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FTreeNode> DetailNode, bool bOverrideRowWidget, TSharedPtr<SWidget> Widget);
		//TSharedPtr<SWidget> CreateCustomDetailRowWidget();
		void SplitterSlotResized(float Size, int32 Index);
		void SetSplitterSlotSize(int32 Index, float Size);

		float GetSlotSize() const;
		FReply OnResetClicked();
		virtual bool HasSplitter();

		TSharedPtr<FPropertyTreeNode> GetPropertyNode() { return StaticCastSharedPtr<FPropertyTreeNode>(NodePtr); };



	private:
		TSharedPtr<SSplitter> Splitter;

		FOnPropertySpiltterSlotResized OnSplitterSlotResized;
	};

}