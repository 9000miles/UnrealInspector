#include "Creater/Vector4Creater.h"
#include "Widgets/Input/SNumericEntryBox.h"

namespace DETAILS_VIEWER
{
#define LOCTEXT_NAMESPACE "SPropertyWidgetVector4"

	void SPropertyWidgetVector4::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);
		const FLinearColor LabelClr = FLinearColor(1.f, 1.f, 1.f, 0.4f);

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
						.Value(this, &SPropertyWidgetVector4::GetPropertyValue_X)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector4::OnValueChanged_X)
						.OnValueCommitted(this, &SPropertyWidgetVector4::OnValueCommitted_X)
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
						.Value(this, &SPropertyWidgetVector4::GetPropertyValue_Y)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector4::OnValueChanged_Y)
						.OnValueCommitted(this, &SPropertyWidgetVector4::OnValueCommitted_Y)
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
						.Value(this, &SPropertyWidgetVector4::GetPropertyValue_Z)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector4::OnValueChanged_Z)
						.OnValueCommitted(this, &SPropertyWidgetVector4::OnValueCommitted_Z)
				]
				+ SHorizontalBox::Slot()
				.Padding(2).HAlign(HAlign_Fill)
				[
					SNew(SNumericEntryBox<float>)
						.LabelVAlign(VAlign_Center)
						.Label()
						[
							SNew(STextBlock)
								.Text(LOCTEXT("Lable_W", "W"))
								.ColorAndOpacity(LabelClr)
						]
						.Value(this, &SPropertyWidgetVector4::GetPropertyValue_W)
						.AllowSpin(true)
						.OnValueChanged(this, &SPropertyWidgetVector4::OnValueChanged_W)
						.OnValueCommitted(this, &SPropertyWidgetVector4::OnValueCommitted_W)
				]
		];
	}

	TOptional<float> SPropertyWidgetVector4::GetPropertyValue_X() const
	{
		FVector4 Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.X = 0;

		return Value.X;
	}

	TOptional<float> SPropertyWidgetVector4::GetPropertyValue_Y() const
	{
		FVector4 Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.Y = 0;

		return Value.Y;
	}


	TOptional<float> SPropertyWidgetVector4::GetPropertyValue_Z() const
	{
		FVector4 Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.Z = 0;

		return Value.Z;
	}

	TOptional<float> SPropertyWidgetVector4::GetPropertyValue_W() const
	{
		FVector4 Value;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value.W = 0;

		return Value.W;
	}


	void SPropertyWidgetVector4::OnValueCommitted_X(float NewValue, ETextCommit::Type CommitType)
	{
		FVector4 Value(
			NewValue,
			GetPropertyValue_Y().Get(0),
			GetPropertyValue_Z().Get(0),
			GetPropertyValue_W().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector4::OnValueCommitted_Y(float NewValue, ETextCommit::Type CommitType)
	{
		FVector4 Value(
			GetPropertyValue_X().Get(0),
			NewValue,
			GetPropertyValue_Z().Get(0),
			GetPropertyValue_W().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}


	void SPropertyWidgetVector4::OnValueCommitted_Z(float NewValue, ETextCommit::Type CommitType)
	{
		FVector4 Value(
			GetPropertyValue_X().Get(0),
			GetPropertyValue_Y().Get(0),
			NewValue,
			GetPropertyValue_W().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector4::OnValueCommitted_W(float NewValue, ETextCommit::Type CommitType)
	{
		FVector4 Value(
			GetPropertyValue_X().Get(0),
			GetPropertyValue_Y().Get(0),
			GetPropertyValue_Z().Get(0),
			NewValue
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}


	void SPropertyWidgetVector4::OnValueChanged_X(float NewValue)
	{
		FVector4 Value(
			NewValue,
			GetPropertyValue_Y().Get(0),
			GetPropertyValue_Z().Get(0),
			GetPropertyValue_W().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector4::OnValueChanged_Y(float NewValue)
	{
		FVector4 Value(
			GetPropertyValue_X().Get(0),
			NewValue,
			GetPropertyValue_Z().Get(0),
			GetPropertyValue_W().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}


	void SPropertyWidgetVector4::OnValueChanged_Z(float NewValue)
	{
		FVector4 Value(
			GetPropertyValue_X().Get(0),
			GetPropertyValue_Y().Get(0),
			NewValue,
			GetPropertyValue_W().Get(0)
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	void SPropertyWidgetVector4::OnValueChanged_W(float NewValue)
	{
		FVector4 Value(
			GetPropertyValue_X().Get(0),
			GetPropertyValue_Y().Get(0),
			GetPropertyValue_Z().Get(0),
			NewValue
		);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	TSharedPtr<SWidget> FWidgetCreaterVector4::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
		return SNew(SPropertyWidgetVector4, PropertyTreeNode->PropertyInfo);
	}

	TArray<FString> FWidgetCreaterVector4::SupportTypes()
	{
		return { TEXT("FVector4"), TEXT("Vector4"), TEXT("vector4") };
	}

#undef LOCTEXT_NAMESPACE
}

