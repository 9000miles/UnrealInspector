// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Node/DetailNode.h"

namespace DETAILS_VIEWER
{
	/**
	 *
	 */
	class DETAILSVIEWER_API SDetailTreeItem : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SDetailTreeItem)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(TSharedPtr<FTreeNode> DetailNode);
		virtual bool HasSplitter();

	protected:
		TSharedPtr<FTreeNode> NodePtr;
	};

}