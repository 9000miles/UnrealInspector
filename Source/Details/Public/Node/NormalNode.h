// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailNode.h"

namespace DETAILS_VIEWER
{

	/**
	 *
	 */
	class DETAILSVIEWER_API FNormalNode :public FDetailTreeNode
	{
	public:
		FNormalNode(UObject* Object, UE_Property* Property);
		virtual ~FNormalNode();

		template<typename T>
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode);

		TSharedPtr<SWidget> GetWidget() override;

	};

	template<typename T>
	TSharedPtr<SWidget> FNormalNode::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		const TSharedPtr<T> Creater = Factory::Get<T>(T::TypeName());
		TSharedPtr<FPropertyWidgetCreater> CreaterPtr = StaticCastSharedPtr<FPropertyWidgetCreater>(Creater);
		return CreaterPtr->MakeWidget();
	}

}