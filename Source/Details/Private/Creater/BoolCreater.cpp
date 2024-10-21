// Fill out your copyright notice in the Description page of Project Settings.

#include "Creater/BoolCreater.h"

namespace DETAILS_VIEWER
{

	void SPropertyWidgetBool::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);

		//UE_Property* Property = PropertyHolder->GetProperty();
		//FBoolProperty* PropertyField = CastField<FBoolProperty>(Property);
		//UObject* Object = PropertyHolder->GetOutermost();
		//const bool Value = PropertyField->GetPropertyValue_InContainer(Object);

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
		//UE_Property* Property = PropertyHolder->GetProperty();
		//FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
		//UObject* Object = PropertyHolder->GetOutermost();
		//const bool Value = BoolProperty->GetPropertyValue_InContainer(Object);

		bool Value = false;
		GetExecutor()->Getter->Get<bool>(Value);
		return Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SPropertyWidgetBool::OnCheckStateChanged(ECheckBoxState State)
	{
		//UE_Property* Property = PropertyHolder->GetProperty();
		//FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
		//UObject* Owner = PropertyHolder->GetOutermost();
		//const bool Value = State == ECheckBoxState::Checked;
		//BoolProperty->SetPropertyValue_InContainer(Owner, Value);

		const bool Value = State == ECheckBoxState::Checked;
		GetExecutor()->Setter->Set(Value);
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


	FString FWidgetCreaterBool::GetTypeName()
	{
		return FWidgetCreaterBool::TypeName();
	}

}