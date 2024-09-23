// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailNode/CategoryNode.h"
#include "DetailCore/DetailDefine.h"

namespace DetailsViewer
{

	FCategoryNode::FCategoryNode(UObject* Object, UE_Property* Property)
		:FDetailTreeNode(EDetailNodeType::Category, Object, Property)
	{

	}

	FCategoryNode::~FCategoryNode()
	{
	}


}