// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailWidget/SPropertyWidgetBool.h"

namespace DetailsViewer
{

	void SPropertyWidgetBool::Construct(const FArguments& InArgs, TSharedPtr<FPropertyHolder> InPropertyProxy)
	{
		SPropertyWidget::Construct(InPropertyProxy);

		UE_Property* Property = PropertyProxy->GetProperty();
		FBoolProperty* PropertyField = CastField<FBoolProperty>(Property);
		UObject* Object = PropertyProxy->GetOutermost();
		const bool Value = PropertyField->GetPropertyValue_InContainer(Object);

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
		UE_Property* Property = PropertyProxy->GetProperty();
		FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
		UObject* Object = PropertyProxy->GetOutermost();
		const bool Value = BoolProperty->GetPropertyValue_InContainer(Object);
		return Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SPropertyWidgetBool::OnCheckStateChanged(ECheckBoxState State)
	{
		UE_Property* Property = PropertyProxy->GetProperty();
		FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
		UObject* Owner = PropertyProxy->GetOutermost();
		const bool Value = State == ECheckBoxState::Checked;
		BoolProperty->SetPropertyValue_InContainer(Owner, Value);
	}
}