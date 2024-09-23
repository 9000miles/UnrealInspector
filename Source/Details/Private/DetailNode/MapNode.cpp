// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailNode/MapNode.h"

namespace DetailsViewer
{
	FMapNode::FMapNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::Map, Object, Property)
	{

	}

	FMapNode::~FMapNode()
	{
	}
}