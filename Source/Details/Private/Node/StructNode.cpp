// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/StructNode.h"

namespace DetailsViewer
{


	FStructNode::FStructNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::Struct, Object, Property)
	{

	}

	FStructNode::~FStructNode()
	{
	}

}