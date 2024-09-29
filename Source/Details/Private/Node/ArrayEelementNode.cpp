// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/ArrayEelementNode.h"

namespace DETAILS_VIEWER
{

	FArrayEelementNode::FArrayEelementNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::ArrayElement, Object, Property)
	{

	}

	FArrayEelementNode::~FArrayEelementNode()
	{
	}

}