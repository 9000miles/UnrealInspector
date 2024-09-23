// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SDetailTreeItem.h"

namespace DetailsViewer
{

	/**
	 *
	 */
	class DETAILSVIEWER_API SDetailCategoryHeader : public SDetailTreeItem
	{
	public:
		SLATE_BEGIN_ARGS(SDetailCategoryHeader)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs);

		bool HasSplitter() override;
	};

}