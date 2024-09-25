// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailCreater/CreaterString.h"
#include "DetailWidget/SPropertyWidget.h"

namespace DetailsViewer
{

	void SPropertyWidgetString::Construct(const FArguments& InArgs, TSharedPtr<FPropertyHolder> InPropertyHolder)
	{
		SPropertyWidget::Construct(InPropertyHolder);

		const FText Value = GetPropertyValue();
		const FText HintText = GetHintText();

		EditableTextBoxPtr = SNew(SEditableTextBox)
			.Text(this, &SPropertyWidgetString::GetText)
			.HintText(HintText)
			.OnTextCommitted(this, &SPropertyWidgetString::OnTextCommitted)
			;

		ChildSlot
			[
				EditableTextBoxPtr.ToSharedRef()
			];
	}

	void SPropertyWidgetString::OnTextCommitted(const FText& Text, ETextCommit::Type Type)
	{
		if (!(Type == ETextCommit::OnEnter || Type == ETextCommit::OnUserMovedFocus)) return;

		SetPropertyValue(Text);
	}

	FText SPropertyWidgetString::GetText() const
	{
		return GetPropertyValue();
	}

	FText SPropertyWidgetString::GetHintText()
	{
		UE_Property* Property = PropertyHolder->GetProperty();
		return Property->GetMetaDataText(TEXT("HintText"));
	}

	FText SPropertyWidgetString::GetPropertyValue() const
	{
		UE_Property* Property = PropertyHolder->GetProperty();
		UObject* Object = PropertyHolder->GetOutermost();
		if (Property->GetCPPType() == TEXT("FString"))
		{
			FStrProperty* PropertyField = CastField<FStrProperty>(Property);
			const FString Value = PropertyField->GetPropertyValue_InContainer(Object);
			return FText::FromString(Value);
		}
		else if (Property->GetCPPType() == TEXT("FName"))
		{
			FNameProperty* PropertyField = CastField<FNameProperty>(Property);
			const FName Value = PropertyField->GetPropertyValue_InContainer(Object);
			return FText::FromName(Value);
		}
		else if (Property->GetCPPType() == TEXT("FText"))
		{
			FTextProperty* PropertyField = CastField<FTextProperty>(Property);
			const FText Value = PropertyField->GetPropertyValue_InContainer(Object);
			return Value;
		}

		return FText();
	}

	void SPropertyWidgetString::SetPropertyValue(FText Text)
	{
		UE_Property* Property = PropertyHolder->GetProperty();
		UObject* Object = PropertyHolder->GetOutermost();
		if (Property->GetCPPType() == TEXT("FString"))
		{
			FStrProperty* PropertyField = CastField<FStrProperty>(Property);
			FString Value = Text.ToString();
			PropertyField->SetPropertyValue_InContainer(Object, Value);
		}
		else if (Property->GetCPPType() == TEXT("FName"))
		{
			FNameProperty* PropertyField = CastField<FNameProperty>(Property);
			FName Value = FName(*Text.ToString());
			PropertyField->SetPropertyValue_InContainer(Object, Value);
		}
		else if (Property->GetCPPType() == TEXT("FText"))
		{
			FTextProperty* PropertyField = CastField<FTextProperty>(Property);
			PropertyField->SetPropertyValue_InContainer(Object, Text);
		}
	}

	FWidgetCreaterString::FWidgetCreaterString()
	{
	}

	TSharedPtr<SWidget> FWidgetCreaterString::MakeWidget()
	{
		return SNew(SPropertyWidgetString, PropertyHolder);
	}

	TArray<FString> FWidgetCreaterString::SupportTypes()
	{
		return { TEXT("FString") ,TEXT("string"), TEXT("FName"), TEXT("FText") };
	}

	FString FWidgetCreaterString::GetTypeName()
	{
		return FWidgetCreaterString::TypeName();
	}

}