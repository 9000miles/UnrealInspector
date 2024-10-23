// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SDetailTreeItem.h"

namespace DETAILS_VIEWER
{

	/**
	 *
	 */
	class DETAILSVIEWER_API SDetailCategory : public SDetailTreeItem
	{
	public:
		SLATE_BEGIN_ARGS(SDetailCategory)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FTreeNode> InTreeNode);

		virtual bool HasSplitter() override;
		virtual void SetSplitterSlotSize(int32 Index, float Size) override {};
	};

}