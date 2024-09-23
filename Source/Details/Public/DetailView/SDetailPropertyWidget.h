// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SDetailTreeItem.h"
#include "Widgets/Layout/SSplitter.h"
#include "DetailNode/DetailNode.h"

DECLARE_DELEGATE_TwoParams(FOnPropertySpiltterSlotResized, int32, float);

namespace DetailsViewer
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
		void Construct(const FArguments& InArgs, TSharedPtr<FDetailTreeNode> DetailNode, bool bOverrideRowWidget, TSharedPtr<SWidget> Widget);
		//TSharedPtr<SWidget> CreateCustomDetailRowWidget();
		void SplitterSlotResized(float Size, int32 Index);
		void SetSplitterSlotSize(int32 Index, float Size);

		float GetSlotSize() const;
		FReply OnResetClicked();
		virtual bool HasSplitter();



	private:
		TSharedPtr<SSplitter> Splitter;

		FOnPropertySpiltterSlotResized OnSplitterSlotResized;
	};

}