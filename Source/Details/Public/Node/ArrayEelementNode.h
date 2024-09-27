// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailNode.h"

namespace DETAILS_VIEWER
{


	/**
	 *
	 */
	class DETAILSVIEWER_API FArrayEelementNode :public FDetailTreeNode
	{
	public:
		FArrayEelementNode(UObject* Object, UE_Property* Property);
		virtual ~FArrayEelementNode();
	};
}