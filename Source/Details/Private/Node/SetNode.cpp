// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/SetNode.h"

namespace DETAILS_VIEWER
{


	FSetNode::FSetNode(UObject* Object, UE_Property* Property)
		:FTreeNode(ENodeType::Set)
	{

	}

	FSetNode::~FSetNode()
	{
	}
}