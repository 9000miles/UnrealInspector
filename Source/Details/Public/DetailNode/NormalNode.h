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

		template<typename T>
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FPropertyHolder> PropertyHolder);

		TSharedPtr<SWidget> GetWidget() override;

	};

	template<typename T>
	TSharedPtr<SWidget> FNormalNode::MakeWidget(TSharedPtr<FPropertyHolder> PropertyHolder)
	{
		const TSharedPtr<T> Creater = Factory::Get<T>(T::TypeName());
		TSharedPtr<FPropertyWidgetCreater> CreaterPtr = StaticCastSharedPtr<FPropertyWidgetCreater>(Creater);
		CreaterPtr->PropertyHolder = PropertyHolder;
		return CreaterPtr->MakeWidget();
	}

}