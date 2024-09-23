// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailNode/MapElementNode.h"

namespace DetailsViewer
{

	FMapElementNode::FMapElementNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::MapElement, Object, Property)
	{

	}

	FMapElementNode::~FMapElementNode()
	{
	}
}