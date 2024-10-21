// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/MapNode.h"

namespace DETAILS_VIEWER
{
	FMapNode::FMapNode(UObject* Object, UE_Property* Property)
		:FTreeNode(ENodeType::Map)
	{

	}

	FMapNode::~FMapNode()
	{
	}
}