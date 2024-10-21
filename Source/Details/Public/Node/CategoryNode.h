// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailNode.h"

namespace DETAILS_VIEWER
{

	/**
	 *
	 */
	class DETAILSVIEWER_API FCategoryNode :public FTreeNode
	{
	public:
		FCategoryNode(TSharedPtr<FCategoryInfo> InCategoryInfo);
		virtual ~FCategoryNode();

	public:
		TSharedPtr<FCategoryInfo> CategoryInfo;
	};

}