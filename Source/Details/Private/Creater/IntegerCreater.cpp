// Fill out your copyright notice in the Description page of Project Settings.


#include "Creater/IntegerCreater.h"
#include "Widget/SPropertyWidget.h"
#include "Widgets/Input/SSpinBox.h"

namespace DETAILS_VIEWER
{

	FWidgetCreaterInteger::FWidgetCreaterInteger()
	{
	}

	TSharedPtr<SWidget> FWidgetCreaterInteger::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
		const FString Type = PropertyTreeNode->PropertyInfo->Type;
		if (Type == TEXT("int"))
		{
			//return SNew(SPropertyWidgetInteger<int>, PropertyTreeNode->PropertyInfo);
		}
		else if (Type == TEXT("uint8"))
		{
			//return SNew(SPropertyWidgetInteger<uint8>, PropertyTreeNode->PropertyInfo);
		}
		else if (Type == TEXT("int32"))
		{
			return SNew(SPropertyWidgetInteger<int32>, PropertyTreeNode->PropertyInfo);
		}

		return SNullWidget::NullWidget;
	}

	TArray<FString> FWidgetCreaterInteger::SupportTypes()
	{
		return { TEXT("int32") ,TEXT("int"), TEXT("int64"), TEXT("uint8") };
	}

	FString FWidgetCreaterInteger::GetTypeName()
	{
		return FWidgetCreaterInteger::TypeName();
	}

}