// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCreater.h"
#include "Core/PropertyHolder.h"

class SPropertyWidgetString;
namespace DetailsViewer
{
	/**
	 *
	 */
	class DETAILSVIEWER_API SPropertyWidgetString :public SPropertyWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidgetString)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyHolder> InPropertyHolder);

		void OnTextCommitted(const FText& Text, ETextCommit::Type Type);

		FText GetText() const;

		FText GetHintText();
		FText GetPropertyValue() const;
		void SetPropertyValue(FText Text);

	private:
		TSharedPtr<SEditableTextBox> EditableTextBoxPtr;
	};
	/**
	 *
	 */
	class DETAILSVIEWER_API FWidgetCreaterString : public FPropertyWidgetCreater
	{
	public:
		FWidgetCreaterString();
		TSharedPtr<SWidget> MakeWidget() override;
		TArray<FString> SupportTypes() override;
		static FString TypeName() { return TEXT("WidgetCreaterString"); }

		FString GetTypeName() override;

	private:
	};

};