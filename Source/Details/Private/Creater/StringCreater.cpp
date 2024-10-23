// Fill out your copyright notice in the Description page of Project Settings.


#include "Creater/StringCreater.h"
#include "Widget/SPropertyWidget.h"

namespace DETAILS_VIEWER
{

	void SPropertyWidgetString::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);

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
		return PropertyInfo->Metadata->Get<FText>(TEXT("HintText"));

		//UE_Property* Property = PropertyHolder->GetProperty();
		//return Property->GetMetaDataText(TEXT("HintText"));
	}

	FText SPropertyWidgetString::GetPropertyValue() const
	{
		const FString Type = PropertyInfo->Type;
		if (Type == TEXT("FString"))
		{
			FString Result;
			GetExecutor()->Getter->Get(Result);
			return FText::FromString(Result);
		}
		else if (Type == TEXT("FName"))
		{
			FName Result;
			GetExecutor()->Getter->Get(Result);
			return FText::FromString(Result.ToString());
		}
		else if (Type == TEXT("FText"))
		{
			FText Result;
			GetExecutor()->Getter->Get(Result);
			return Result;
		}

		FText Result;
		GetExecutor()->Getter->Get(Result);
		return Result;

		//UE_Property* Property = PropertyHolder->GetProperty();
		//UObject* Object = PropertyHolder->GetOutermost();
		//if (Property->GetCPPType() == TEXT("FString"))
		//{
		//	FStrProperty* PropertyField = CastField<FStrProperty>(Property);
		//	const FString Value = PropertyField->GetPropertyValue_InContainer(Object);
		//	return FText::FromString(Value);
		//}
		//else if (Property->GetCPPType() == TEXT("FName"))
		//{
		//	FNameProperty* PropertyField = CastField<FNameProperty>(Property);
		//	const FName Value = PropertyField->GetPropertyValue_InContainer(Object);
		//	return FText::FromName(Value);
		//}
		//else if (Property->GetCPPType() == TEXT("FText"))
		//{
		//	FTextProperty* PropertyField = CastField<FTextProperty>(Property);
		//	const FText Value = PropertyField->GetPropertyValue_InContainer(Object);
		//	return Value;
		//}

		//return FText();
	}

	void SPropertyWidgetString::SetPropertyValue(FText Text)
	{
		const FString Type = PropertyInfo->Type;
		if (Type == TEXT("FString"))
		{
			GetExecutor()->Setter->Set(Text.ToString());

			//FStrProperty* PropertyField = CastField<FStrProperty>(Property);
			//FString Value = Text.ToString();
			//PropertyField->SetPropertyValue_InContainer(Object, Value);
		}
		else if (Type == TEXT("FName"))
		{
			GetExecutor()->Setter->Set(FName(*Text.ToString()));
			//FNameProperty* PropertyField = CastField<FNameProperty>(Property);
			//FName Value = FName(*Text.ToString());
			//PropertyField->SetPropertyValue_InContainer(Object, Value);
		}
		else if (Type == TEXT("FText"))
		{
			GetExecutor()->Setter->Set(Text);
			//FTextProperty* PropertyField = CastField<FTextProperty>(Property);
			//PropertyField->SetPropertyValue_InContainer(Object, Text);
		}
	}

	FWidgetCreaterString::FWidgetCreaterString()
	{
	}

	TSharedPtr<SWidget> FWidgetCreaterString::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
		return SNew(SPropertyWidgetString, PropertyTreeNode->PropertyInfo);
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