// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/StructNode.h"

namespace DETAILS_VIEWER
{


	FStructNode::FStructNode(UObject* Object, UE_Property* Property)
		:FTreeNode(ENodeType::Struct)
	{

	}

	FStructNode::~FStructNode()
	{
	}

}