// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Core/PropertyHolder.h"
#include "Core/DetailInfo.h"

namespace DETAILS_VIEWER
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
		void Construct(TSharedPtr<FPropertyInfo> InPropertyInfo);

	protected:
		TSharedPtr<PROPERTY::IExecutor> GetExecutor() const { return PropertyInfo->Executor; }


	protected:
		TSharedPtr<FPropertyInfo> PropertyInfo;
	};

}
