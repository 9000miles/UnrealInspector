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
	class DETAILSVIEWER_API SPropertyWidgetGuid :public SPropertyWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidgetGuid)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo);

		void OnTextCommitted(const FText& Text, ETextCommit::Type Type);

		FText GetPropertyValue() const;
		FReply OnButtonClicked();

		void OnSelectionChanged(FName InSelectedItem, ESelectInfo::Type SelectInfo);
		TSharedRef<SWidget> OnGenerateComboWidget(FName InComboString);

	private:
		EGuidFormats GuidFormat;
		TMap<FName, EGuidFormats> GuidFormatMap;
	};
	/**
	 *
	 */
	class DETAILSVIEWER_API FWidgetCreaterGuid : public FPropertyWidgetCreater
	{
		IMPLEMENT_ITYPENAME(FWidgetCreaterGuid)
	public:
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;

		bool IsAllowHasChildren() override { return false; }
		TArray<FString> SupportTypes() override;

	private:
	};

};