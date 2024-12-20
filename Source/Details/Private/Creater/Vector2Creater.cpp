#include "Creater/Vector2Creater.h"
#include "Widgets/Input/SNumericEntryBox.h"

namespace DETAILS_VIEWER
{
#define LOCTEXT_NAMESPACE "SPropertyWidgetVector2"

	void SPropertyWidgetVector2::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);
		const FLinearColor LabelClr = FLinearColor(1.f, 1.f, 1.f, 0.4f);

		ChildSlot[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				//.AutoWidth()
				.Padding(2).HAlign(HAlign_Fill)
				[
					SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.Label()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("VectorNodeXAxisValueLabel", "X"))
								.ColorAndOpacity(LabelClr)
						]
						.Value(this, &SPropertyWidgetVector2::GetPropertyValue_X)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector2::OnValueChanged_X)
						.OnValueCommitted(this, &SPropertyWidgetVector2::OnValueCommitted_X)
				]
				+ SHorizontalBox::Slot()
				//.AutoWidth()
				.Padding(2).HAlign(HAlign_Fill)
				[
					SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.Label()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("VectorNodeYAxisValueLabel", "Y"))
								.ColorAndOpacity(LabelClr)
						]
						.Value(this, &SPropertyWidgetVector2::GetPropertyValue_Y)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector2::OnValueChanged_Y)
						.OnValueCommitted(this, &SPropertyWidgetVector2::OnValueCommitted_Y)
				]
		];
	}

	TOptional<float> SPropertyWidgetVector2::GetPropertyValue_X() const
	{
		FVector2D Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.X = 0;

		return Value.X;
	}

	TOptional<float> SPropertyWidgetVector2::GetPropertyValue_Y() const
	{
		FVector2D Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.Y = 0;

		return Value.Y;
	}

	void SPropertyWidgetVector2::OnValueCommitted_X(float NewValue, ETextCommit::Type CommitType)
	{
		FVector2D Value(NewValue, GetPropertyValue_Y().Get(0));
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector2::OnValueCommitted_Y(float NewValue, ETextCommit::Type CommitType)
	{
		FVector2D Value(GetPropertyValue_X().Get(0), NewValue);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector2::OnValueChanged_X(float NewValue)
	{
		FVector2D Value(NewValue, GetPropertyValue_Y().Get(0));
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector2::OnValueChanged_Y(float NewValue)
	{
		FVector2D Value(GetPropertyValue_X().Get(0), NewValue);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	TSharedPtr<SWidget> FWidgetCreaterVector2::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
		return SNew(SPropertyWidgetVector2, PropertyTreeNode->PropertyInfo);
	}

	TArray<FString> FWidgetCreaterVector2::SupportTypes()
	{
		return { TEXT("FVector2D"), TEXT("Vector2"), TEXT("vector2") };
	}

#undef LOCTEXT_NAMESPACE
}

