// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailNode/SetNode.h"

namespace DetailsViewer
{


	FSetNode::FSetNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::Set, Object, Property)
	{

	}

	FSetNode::~FSetNode()
	{
	}
}