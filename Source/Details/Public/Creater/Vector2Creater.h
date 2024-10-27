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
	class DETAILSVIEWER_API SPropertyWidgetVector2 :public SPropertyWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidgetVector2)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo);
		TOptional<float> GetPropertyValue_X() const;
		TOptional<float> GetPropertyValue_Y() const;
		void OnValueCommitted_X(float NewValue, ETextCommit::Type CommitType);
		void OnValueCommitted_Y(float NewValue, ETextCommit::Type CommitType);
		void OnValueChanged_X(float NewValue);
		void OnValueChanged_Y(float NewValue);

	};
	/**
	 *
	 */
	class DETAILSVIEWER_API FWidgetCreaterVector2 : public FPropertyWidgetCreater
	{
		IMPLEMENT_ITYPENAME(FWidgetCreaterVector2)
	public:
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;
		TArray<FString> SupportTypes() override;

	private:
	};

};