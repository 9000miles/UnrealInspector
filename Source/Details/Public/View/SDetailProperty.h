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
	class DETAILSVIEWER_API SDetailProperty : public SDetailTreeItem
	{
	public:
		SLATE_BEGIN_ARGS(SDetailProperty)
			{}
			SLATE_EVENT(FOnPropertySpiltterSlotResized, OnSplitterSlotResized)
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FTreeNode> InTreeNode, bool bOverrideRowWidget, TSharedPtr<SWidget> Widget);
		//TSharedPtr<SWidget> CreateCustomDetailRowWidget();
		void SplitterSlotResized(float Size, int32 Index);
		virtual void SetSplitterSlotSize(int32 Index, float Size) override;
		virtual bool HasSplitter() override;

		float GetSlotSize() const;
		FReply OnResetClicked();


	private:
		TSharedPtr<SSplitter> Splitter;
		FOnPropertySpiltterSlotResized OnSplitterSlotResized;
	};

}