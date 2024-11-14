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
		//template<typename T>
		void Construct(TSharedPtr<FTreeNode> DetailNode);
		virtual bool HasSplitter() { return false; };
		virtual void SetSplitterSlotSize(int32 Index, float Size) {};
		virtual float GetSplitterSlotSize() { return 0.1f; };
		template<typename T>
		TSharedPtr<T> GetNode() const { return StaticCastSharedPtr<T>(NodePtr); };

	protected:
		TSharedPtr<FTreeNode> NodePtr;
	};

}