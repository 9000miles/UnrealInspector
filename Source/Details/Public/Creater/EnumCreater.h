// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCreater.h"
#include "Core/PropertyHolder.h"
#include "Core/DetailInfo.h"

namespace DETAILS_VIEWER
{
	/**
	 *
	 */
	class DETAILSVIEWER_API SPropertyWidgetEnum :public SPropertyWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidgetEnum)
			{}
		SLATE_END_ARGS()
		void OnSelectionChanged(FName InSelectedItem, ESelectInfo::Type SelectInfo);
		TSharedRef<SWidget> OnGenerateComboWidget(FName InComboString);
		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo);
		FText GetPropertyValue() const;

	private:
		TArray<FName> EnumNames;
	};
	/**
	 *
	 */
	class DETAILSVIEWER_API FWidgetCreaterEnum : public FPropertyWidgetCreater
	{
		IMPLEMENT_ITYPENAME(FWidgetCreaterEnum)
	public:
		FWidgetCreaterEnum();
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;
		TArray<FString> SupportTypes() override;
	private:
	};

};