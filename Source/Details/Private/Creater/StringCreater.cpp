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
	}

	FText SPropertyWidgetString::GetPropertyValue() const
	{
		const FString Type = PropertyInfo->Type;
		if (Type == TEXT("FString"))
		{
			FString Result;
			GetExecutor()->Accessor->Get(Result);
			return FText::FromString(Result);
		}
		else if (Type == TEXT("string"))
		{
			FString Result;
			GetExecutor()->Accessor->Get(Result);
			return FText::FromString(Result);
		}
		else if (Type == TEXT("FName"))
		{
			FName Result;
			GetExecutor()->Accessor->Get(Result);
			return FText::FromString(Result.ToString());
		}
		else if (Type == TEXT("FText"))
		{
			FText Result;
			GetExecutor()->Accessor->Get(Result);
			return Result;
		}

		return FText();
	}

	void SPropertyWidgetString::SetPropertyValue(FText Text)
	{
		const FString Type = PropertyInfo->Type;
		if (Type == TEXT("FString"))
		{
			GetExecutor()->Accessor->Set(Text.ToString());
		}
		else if (Type == TEXT("string"))
		{
			GetExecutor()->Accessor->Set(Text.ToString());
		}
		else if (Type == TEXT("FName"))
		{
			GetExecutor()->Accessor->Set(FName(*Text.ToString()));
		}
		else if (Type == TEXT("FText"))
		{
			GetExecutor()->Accessor->Set(Text);
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