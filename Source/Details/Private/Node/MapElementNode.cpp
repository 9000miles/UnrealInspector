// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/MapElementNode.h"

namespace DETAILS_VIEWER
{

	FMapElementNode::FMapElementNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::MapElement, Object, Property)
	{

	}

	FMapElementNode::~FMapElementNode()
	{
	}
}