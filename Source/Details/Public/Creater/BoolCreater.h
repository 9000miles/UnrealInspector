// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCreater.h"
#include "Widget/SPropertyWidget.h"
#include "Core/DetailInfo.h"

namespace DETAILS_VIEWER
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
		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo);
		ECheckBoxState IsChecked()const;
		void OnCheckStateChanged(ECheckBoxState State);

	private:
		TSharedPtr<SCheckBox> CheckBoxPtr;
	};

	class DETAILSVIEWER_API FWidgetCreaterBool : public FPropertyWidgetCreater
	{
		IMPLEMENT_ITYPENAME(FWidgetCreaterBool)
	public:
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;
		TArray<FString> SupportTypes() override;

		static bool IsSupport(TSharedPtr<FPropertyInfo> PropertyInfo)
		{
			const TArray<FString> Typies = Factory::Get<IDetailWidgetCreater>(FWidgetCreaterBool::TypeName())->SupportTypes();
			return Typies.ContainsByPredicate([PropertyInfo](const FString& Type) { return Type == PropertyInfo->Type; });
		}

	};
};
