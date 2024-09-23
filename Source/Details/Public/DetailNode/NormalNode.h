// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailNode.h"

namespace DetailsViewer
{

	/**
	 *
	 */
	class DETAILSVIEWER_API FNormalNode :public FDetailTreeNode
	{
	public:
		FNormalNode(UObject* Object, UE_Property* Property);
		virtual ~FNormalNode();
	};

}