#include "Creater/Vector3Creater.h"
#include "Widgets/Input/SNumericEntryBox.h"

namespace DETAILS_VIEWER
{
#define LOCTEXT_NAMESPACE "SPropertyWidgetVector"

	void SPropertyWidgetVector::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);
		const FLinearColor LabelClr = FLinearColor(1.f, 1.f, 1.f, 0.f);

		ChildSlot[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(2).HAlign(HAlign_Fill)
				[
					SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.Label()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("Lable_X", "X"))
								.ColorAndOpacity(LabelClr)
						]
						.Value(this, &SPropertyWidgetVector::GetPropertyValue_X)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector::OnValueChanged_X)
						.OnValueCommitted(this, &SPropertyWidgetVector::OnValueCommitted_X)
				]
				+ SHorizontalBox::Slot()
				.Padding(2).HAlign(HAlign_Fill)
				[
					SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.Label()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("Lable_Y", "Y"))
								.ColorAndOpacity(LabelClr)
						]
						.Value(this, &SPropertyWidgetVector::GetPropertyValue_Y)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector::OnValueChanged_Y)
						.OnValueCommitted(this, &SPropertyWidgetVector::OnValueCommitted_Y)
				]
				+ SHorizontalBox::Slot()
				.Padding(2).HAlign(HAlign_Fill)
				[
					SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.Label()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("Lable_Z", "Z"))
								.ColorAndOpacity(LabelClr)
						]
						.Value(this, &SPropertyWidgetVector::GetPropertyValue_Z)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector::OnValueChanged_Z)
						.OnValueCommitted(this, &SPropertyWidgetVector::OnValueCommitted_Z)
				]
		];
	}

	TOptional<float> SPropertyWidgetVector::GetPropertyValue_X() const
	{
		FVector Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.X = 0;

		return Value.X;
	}

	TOptional<float> SPropertyWidgetVector::GetPropertyValue_Y() const
	{
		FVector Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.Y = 0;

		return Value.Y;
	}


	TOptional<float> SPropertyWidgetVector::GetPropertyValue_Z() const
	{
		FVector Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.Z = 0;

		return Value.Z;
	}

	void SPropertyWidgetVector::OnValueCommitted_X(float NewValue, ETextCommit::Type CommitType)
	{
		FVector Value(
			NewValue,
			GetPropertyValue_Y().Get(0),
			GetPropertyValue_Z().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector::OnValueCommitted_Y(float NewValue, ETextCommit::Type CommitType)
	{
		FVector Value(
			GetPropertyValue_X().Get(0),
			NewValue,
			GetPropertyValue_Z().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}


	void SPropertyWidgetVector::OnValueCommitted_Z(float NewValue, ETextCommit::Type CommitType)
	{
		FVector Value(
			GetPropertyValue_X().Get(0),
			GetPropertyValue_Y().Get(0),
			NewValue
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector::OnValueChanged_X(float NewValue)
	{
		FVector Value(
			NewValue,
			GetPropertyValue_Y().Get(0),
			GetPropertyValue_Z().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector::OnValueChanged_Y(float NewValue)
	{
		FVector Value(
			GetPropertyValue_X().Get(0),
			NewValue,
			GetPropertyValue_Z().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}


	void SPropertyWidgetVector::OnValueChanged_Z(float NewValue)
	{
		FVector Value(
			GetPropertyValue_X().Get(0),
			GetPropertyValue_Y().Get(0),
			NewValue
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	TSharedPtr<SWidget> FWidgetCreaterVector::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
		return SNew(SPropertyWidgetVector, PropertyTreeNode->PropertyInfo);
	}

	TArray<FString> FWidgetCreaterVector::SupportTypes()
	{
		return { TEXT("FVector"), TEXT("Vector3"), TEXT("vector3") };
	}

#undef LOCTEXT_NAMESPACE
}

