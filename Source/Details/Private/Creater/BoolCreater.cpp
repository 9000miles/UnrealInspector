// Fill out your copyright notice in the Description page of Project Settings.

#include "Creater/BoolCreater.h"
#include "Core/DetailInfo.h"

namespace DETAILS_VIEWER
{

	void SPropertyWidgetBool::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);

		CheckBoxPtr = SNew(SCheckBox)
			.IsChecked(this, &SPropertyWidgetBool::IsChecked)
			.OnCheckStateChanged(this, &SPropertyWidgetBool::OnCheckStateChanged)
			;

		ChildSlot
			[
				CheckBoxPtr.ToSharedRef()
			];
	}

	ECheckBoxState SPropertyWidgetBool::IsChecked() const
	{
		bool Value = false;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value = false;

		return Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SPropertyWidgetBool::OnCheckStateChanged(ECheckBoxState State)
	{
		const bool Value = State == ECheckBoxState::Checked;
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	TSharedPtr<SWidget> FWidgetCreaterBool::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
		return SNew(SPropertyWidgetBool, PropertyTreeNode->PropertyInfo);

	}

	TArray<FString> FWidgetCreaterBool::SupportTypes()
	{
		return { TEXT("bool"), TEXT("Bool"), TEXT("boolean") };
	}
}