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
			if (!GetAccessor()->Get(&Result, sizeof(FString)))
				Result = TEXT("");

			return FText::FromString(Result);
		}
		else if (Type == TEXT("string"))
		{
			FString Result;
			if (!GetAccessor()->Get(&Result, sizeof(FString)))
				Result = TEXT("");

			return FText::FromString(Result);
		}
		else if (Type == TEXT("FName"))
		{
			FName Result;
			if (!GetAccessor()->Get(&Result, sizeof(FName)))
				Result = FName();

			return FText::FromString(Result.ToString());
		}
		else if (Type == TEXT("FText"))
		{
			FText Result;
			if (!GetAccessor()->Get(&Result, sizeof(FText)))
				Result = FText();

			return Result;
		}

		return FText();
	}

	void SPropertyWidgetString::SetPropertyValue(FText Text)
	{
		const FString Type = PropertyInfo->Type;
		if (Type == TEXT("FString"))
		{
			GetAccessor()->Set(&Text.ToString(), sizeof(FString));
		}
		else if (Type == TEXT("string"))
		{
			GetAccessor()->Set(&Text.ToString(), sizeof(FString));
		}
		else if (Type == TEXT("FName"))
		{
			FName Value(*Text.ToString());
			GetAccessor()->Set(&Value, sizeof(FName));
		}
		else if (Type == TEXT("FText"))
		{
			GetAccessor()->Set(&Text, sizeof(FText));
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
}