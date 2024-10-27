// Fill out your copyright notice in the Description page of Project Settings.


#include "Creater/NumericCreater.h"
#include "Widget/SPropertyWidget.h"
#include "Widgets/Input/SSpinBox.h"

namespace DETAILS_VIEWER
{

	FWidgetCreaterNumeric::FWidgetCreaterNumeric()
	{
	}

	TSharedPtr<SWidget> FWidgetCreaterNumeric::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
		const FString Type = PropertyTreeNode->PropertyInfo->Type;
		if (Type == TEXT("float")) {
			return SNew(SPropertyWidgetNumeric<float>, PropertyTreeNode->PropertyInfo);
		}
		else if (Type == TEXT("double")) {
			return SNew(SPropertyWidgetNumeric<double>, PropertyTreeNode->PropertyInfo);
		}
		else if (Type == TEXT("int") || Type == TEXT("int32")) {
			return SNew(SPropertyWidgetNumeric<int32>, PropertyTreeNode->PropertyInfo);
		}

		return SNullWidget::NullWidget;
	}

	TArray<FString> FWidgetCreaterNumeric::SupportTypes()
	{
		return {
			TEXT("uint8"),
			TEXT("int"),
			TEXT("int32"),
			TEXT("int64"),
			TEXT("float"),
			TEXT("double")
		};
	}
}