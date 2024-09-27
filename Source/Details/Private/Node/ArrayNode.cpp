// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/ArrayNode.h"
namespace DETAILS_VIEWER
{

	FArrayNode::FArrayNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::Array, Object, Property)
	{

	}

	FArrayNode::~FArrayNode()
	{
	}

}