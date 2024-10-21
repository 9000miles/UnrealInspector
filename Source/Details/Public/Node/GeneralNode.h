// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailNode.h"

namespace DETAILS_VIEWER
{

	/**
	 *
	 */
	class DETAILSVIEWER_API FGeneralNode :public FTreeNode
	{
	public:
		FGeneralNode(UObject* InObject, UE_Property* InProperty);
		virtual ~FGeneralNode();

		template<typename T>
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode);

		TSharedPtr<SWidget> GetWidget() override;

	public:
		TWeakObjectPtr<UObject> Object;
		UE_Property* Property;
	};

	template<typename T>
	TSharedPtr<SWidget> FGeneralNode::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		const TSharedPtr<T> Creater = Factory::Get<T>(T::TypeName());
		TSharedPtr<FPropertyWidgetCreater> CreaterPtr = StaticCastSharedPtr<FPropertyWidgetCreater>(Creater);
		return CreaterPtr->MakeWidget();
	}

}