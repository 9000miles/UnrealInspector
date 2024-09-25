// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCreater.h"
#include "DetailWidget/SPropertyWidget.h"

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
		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyHolder> InPropertyHolder);
		ECheckBoxState IsChecked()const;
		void OnCheckStateChanged(ECheckBoxState State);

	private:
		TSharedPtr<SCheckBox> CheckBoxPtr;
	};

	class DETAILSVIEWER_API FWidgetCreaterBool : public FPropertyWidgetCreater
	{
	public:
		TSharedPtr<SWidget> MakeWidget() override;
		TArray<FString> SupportTypes() override;

		static FString TypeName() { return TEXT("WidgetCreaterBool"); }
		FString GetTypeName() override;

	};
};