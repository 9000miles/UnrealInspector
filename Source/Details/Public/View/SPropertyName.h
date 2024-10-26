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
	class DETAILSVIEWER_API SPropertyName : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyName)
			{}
		SLATE_END_ARGS()

		FString GetDisplayName();
		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FTreeNode> TreeNode);

	private:
		TSharedPtr<FTreeNode> TreeNode;
	};
}