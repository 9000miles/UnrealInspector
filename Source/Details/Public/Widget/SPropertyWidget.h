// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/PropertyHolder.h"

namespace DetailsViewer
{

	/**
	 *
	 */
	class DETAILSVIEWER_API SPropertyWidget :public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidget)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(TSharedPtr<FPropertyHolder> InPropertyHolder);

	protected:
		TSharedPtr<FPropertyHolder> PropertyHolder;
	};

}
