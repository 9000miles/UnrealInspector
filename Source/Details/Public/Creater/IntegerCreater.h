// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCreater.h"
#include "Core/PropertyHolder.h"
#include "Core/DetailInfo.h"
#include "Widgets/Input/SSpinBox.h"

namespace DETAILS_VIEWER
{
	/**
	 *
	 */
	template<typename T>
	class DETAILSVIEWER_API SPropertyWidgetInteger :public SPropertyWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidgetInteger)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */

		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
		{
			SPropertyWidget::Construct(InPropertyInfo);

			ChildSlot
				[
					SNew(SSpinBox<T>)
						.OnValueChanged(this, &SPropertyWidgetInteger::OnValueChanged)
						.OnValueCommitted(this, &SPropertyWidgetInteger::OnValueCommitted)
						.Value(this, &SPropertyWidgetInteger::GetPropertyValue)
				];
		}

		void OnValueChanged(T Value)
		{
			GetExecutor()->Accessor->Set(Value);
		}
		void OnValueCommitted(T Value, ETextCommit::Type CommitType)
		{
			GetExecutor()->Accessor->Set(Value);
		}

		T GetPropertyValue() const
		{
			T Value;
			GetExecutor()->Accessor->Get(Value);
			return Value;
		}

	private:
		TSharedPtr<SEditableTextBox> EditableTextBoxPtr;
	};

	/**
	 *
	 */
	class DETAILSVIEWER_API FWidgetCreaterInteger : public FPropertyWidgetCreater
	{
	public:
		FWidgetCreaterInteger();
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;
		TArray<FString> SupportTypes() override;
		static FString TypeName() { return TEXT("WidgetCreaterInteger"); }

		FString GetTypeName() override;

	private:
	};

};