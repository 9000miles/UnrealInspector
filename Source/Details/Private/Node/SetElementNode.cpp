// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/SetElementNode.h"

namespace DETAILS_VIEWER
{

	FSetElementNode::FSetElementNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::SetElement, Object, Property)
	{

	}

	FSetElementNode::~FSetElementNode()
	{
	}
}