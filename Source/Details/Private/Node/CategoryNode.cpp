// Fill out your copyright notice in the Description page of Project Settings.


#include "Node/CategoryNode.h"
#include "Core/DetailDefine.h"

namespace DETAILS_VIEWER
{

	FCategoryNode::FCategoryNode(TSharedPtr<FCategoryInfo> InCategoryInfo)
		:FTreeNode(ENodeType::Category),
		CategoryInfo(InCategoryInfo)
	{

	}

	FCategoryNode::~FCategoryNode()
	{
	}


}