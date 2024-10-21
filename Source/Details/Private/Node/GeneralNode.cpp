// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/GeneralNode.h"
#include "Creater/BoolCreater.h"
#include "Creater/StringCreater.h"

namespace DETAILS_VIEWER
{
	FGeneralNode::FGeneralNode(UObject* InObject, UE_Property* InProperty)
		:FTreeNode(ENodeType::General),
		Object(InObject),
		Property(InProperty)
	{

	}

	FGeneralNode::~FGeneralNode()
	{
	}


	TSharedPtr<SWidget> FGeneralNode::GetWidget()
	{
		//if (PropertyHolder->IsA(FBoolProperty::StaticClass()))
		//	return MakeWidget<FWidgetCreaterBool>(PropertyHolder);
		////else if (PropertyHolder->IsA(FIntProperty::StaticClass()))
		////	return FWidgetCreaterInt::CreateWidget(PropertyHolder);
		////else if (PropertyHolder->IsA(FInt64Property::StaticClass()))
		////	return FWidgetCreaterInt64::CreateWidget(PropertyHolder);
		////else if (PropertyHolder->IsA(FFloatProperty::StaticClass()))
		////	return FWidgetCreaterFloat::CreateWidget(PropertyHolder);
		////else if (PropertyHolder->IsA(FDoubleProperty::StaticClass()))
		////	return FWidgetCreaterDouble::CreateWidget(PropertyHolder);
		//else if (PropertyHolder->IsA(FStrProperty::StaticClass())
		//	|| PropertyHolder->IsA(FNameProperty::StaticClass()))
		//	return MakeWidget<FWidgetCreaterString>(PropertyHolder);


		return FTreeNode::GetWidget();
	}


}