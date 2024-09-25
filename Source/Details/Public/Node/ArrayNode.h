// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailNode.h"


namespace DetailsViewer
{
	/**
	 *
	 */
	class DETAILSVIEWER_API FArrayNode :public FDetailTreeNode
	{
	public:
		FArrayNode(UObject* Object, UE_Property* Property);
		virtual ~FArrayNode();
	};
}