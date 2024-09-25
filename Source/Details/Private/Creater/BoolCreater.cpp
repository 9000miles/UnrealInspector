// Fill out your copyright notice in the Description page of Project Settings.

#include "Creater/BoolCreater.h"

namespace DetailsViewer
{

	void SPropertyWidgetBool::Construct(const FArguments& InArgs, TSharedPtr<FPropertyHolder> InPropertyHolder)
	{
		SPropertyWidget::Construct(InPropertyHolder);

		UE_Property* Property = PropertyHolder->GetProperty();
		FBoolProperty* PropertyField = CastField<FBoolProperty>(Property);
		UObject* Object = PropertyHolder->GetOutermost();
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
		UE_Property* Property = PropertyHolder->GetProperty();
		FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
		UObject* Object = PropertyHolder->GetOutermost();
		const bool Value = BoolProperty->GetPropertyValue_InContainer(Object);
		return Value ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SPropertyWidgetBool::OnCheckStateChanged(ECheckBoxState State)
	{
		UE_Property* Property = PropertyHolder->GetProperty();
		FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property);
		UObject* Owner = PropertyHolder->GetOutermost();
		const bool Value = State == ECheckBoxState::Checked;
		BoolProperty->SetPropertyValue_InContainer(Owner, Value);
	}

	TSharedPtr<SWidget> FWidgetCreaterBool::MakeWidget()
	{
		return SNew(SPropertyWidgetBool, PropertyHolder);
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