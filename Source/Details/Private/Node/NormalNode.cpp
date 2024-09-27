// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/NormalNode.h"
#include "Creater/BoolCreater.h"
#include "Creater/StringCreater.h"

namespace DETAILS_VIEWER
{
	FNormalNode::FNormalNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::NormalProperty, Object, Property)
	{

	}

	FNormalNode::~FNormalNode()
	{
	}


	TSharedPtr<SWidget> FNormalNode::GetWidget()
	{
		if (PropertyHolder->IsA(FBoolProperty::StaticClass()))
			return MakeWidget<FWidgetCreaterBool>(PropertyHolder);
		//else if (PropertyHolder->IsA(FIntProperty::StaticClass()))
		//	return FWidgetCreaterInt::CreateWidget(PropertyHolder);
		//else if (PropertyHolder->IsA(FInt64Property::StaticClass()))
		//	return FWidgetCreaterInt64::CreateWidget(PropertyHolder);
		//else if (PropertyHolder->IsA(FFloatProperty::StaticClass()))
		//	return FWidgetCreaterFloat::CreateWidget(PropertyHolder);
		//else if (PropertyHolder->IsA(FDoubleProperty::StaticClass()))
		//	return FWidgetCreaterDouble::CreateWidget(PropertyHolder);
		else if (PropertyHolder->IsA(FStrProperty::StaticClass())
			|| PropertyHolder->IsA(FNameProperty::StaticClass()))
			return MakeWidget<FWidgetCreaterString>(PropertyHolder);


		return FDetailTreeNode::GetWidget();
	}


}