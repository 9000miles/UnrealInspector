// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailNode/NormalNode.h"

namespace DetailsViewer
{
	FNormalNode::FNormalNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::NormalProperty, Object, Property)
	{

	}

	FNormalNode::~FNormalNode()
	{
	}

}