// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailNode/ArrayNode.h"
namespace DetailsViewer
{

	FArrayNode::FArrayNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::Array, Object, Property)
	{

	}

	FArrayNode::~FArrayNode()
	{
	}

}