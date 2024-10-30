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
	template<typename NumericType>
	class DETAILSVIEWER_API SPropertyWidgetNumeric :public SPropertyWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidgetNumeric)
			{}
		SLATE_END_ARGS()

		template<typename T>
		struct TOptionalOptions {
			TOptionalOptions(bool bSet = false, T InValue = {}) : Set(bSet), Value(InValue) {}
			bool Set;
			T Value;
		};

		/** Constructs this widget with InArgs */

		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
		{
			SPropertyWidget::Construct(InPropertyInfo);

			const TOptional<NumericType> MinValue = GetMetadataValue<NumericType>(TEXT("Min"), TOptionalOptions<NumericType>());
			const TOptional<NumericType> MaxValue = GetMetadataValue<NumericType>(TEXT("Max"), TOptionalOptions<NumericType>());
			const TOptional<int32> MinDigits = GetMetadataValue<int32>(TEXT("MinDigits"), TOptionalOptions<int32>(true, 1));
			const TOptional<int32> MaxDigits = GetMetadataValue<int32>(TEXT("MaxDigits"), TOptionalOptions<int32>(true, 6));
			const TOptional<NumericType> Delta = GetMetadataValue<NumericType>(TEXT("Delta"), TOptionalOptions<NumericType>());

			ChildSlot
				[
					SNew(SSpinBox<NumericType>)
						.Value(this, &SPropertyWidgetNumeric::GetPropertyValue)
						//.MinValue(MinValue)
						//.MaxValue(MaxValue)
						//.MinSliderValue(MinValue)
						//.MaxSliderValue(MaxValue)
						.MinFractionalDigits(MinDigits)
						.MaxFractionalDigits(MaxDigits)
						//.Delta(Delta.GetValue())
						.OnValueChanged(this, &SPropertyWidgetNumeric::OnValueChanged)
						.OnValueCommitted(this, &SPropertyWidgetNumeric::OnValueCommitted)
				];
		}

		template<typename T>
		TOptional<T> GetMetadataValue(const FString& Key, TOptionalOptions<T> Default) const
		{
			if (PropertyInfo->Metadata->Has(Key))
				return TOptional<T>(PropertyInfo->Metadata->Get<T>(Key));

			return Default.Set ? TOptional<T>(Default.Value) : TOptional<T>();
		}

		void OnValueChanged(NumericType Value)
		{
			GetExecutor()->Accessor->Set(Value);
		}
		void OnValueCommitted(NumericType Value, ETextCommit::Type CommitType)
		{
			GetExecutor()->Accessor->Set(Value);
		}

		NumericType GetPropertyValue() const
		{
			NumericType Value;
			GetExecutor()->Accessor->Get(Value);
			return Value;
		}

	private:
		TSharedPtr<SEditableTextBox> EditableTextBoxPtr;
	};

	/**
	 *
	 */
	class DETAILSVIEWER_API FWidgetCreaterNumeric : public FPropertyWidgetCreater
	{
		IMPLEMENT_ITYPENAME(FWidgetCreaterNumeric)
	public:
		FWidgetCreaterNumeric();
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;
		TArray<FString> SupportTypes() override;

	private:
	};

};