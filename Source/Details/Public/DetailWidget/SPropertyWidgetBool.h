// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPropertyWidget.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SCheckBox.h"

namespace DetailsViewer
{

	/**
	 *
	 */
	class DETAILSVIEWER_API SPropertyWidgetBool :public SPropertyWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidgetBool)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyHolder> InPropertyProxy);
		ECheckBoxState IsChecked()const;
		void OnCheckStateChanged(ECheckBoxState State);

	private:
		TSharedPtr<SCheckBox> CheckBoxPtr;
	};

}