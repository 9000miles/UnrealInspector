// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailNode/SetElementNode.h"

namespace DetailsViewer
{

	FSetElementNode::FSetElementNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::SetElement, Object, Property)
	{

	}

	FSetElementNode::~FSetElementNode()
	{
	}
}