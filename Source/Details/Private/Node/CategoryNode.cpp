// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/CategoryNode.h"
#include "Core/DetailDefine.h"

namespace DETAILS_VIEWER
{

	FCategoryNode::FCategoryNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::Category, Object, Property)
	{

	}

	FCategoryNode::~FCategoryNode()
	{
	}


}