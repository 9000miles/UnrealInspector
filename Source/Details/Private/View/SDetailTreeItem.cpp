// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SDetailTreeItem.h"
#include "SlateOptMacros.h"
#include "View/SPropertyName.h"

namespace DETAILS_VIEWER
{


	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
		void SDetailTreeItem::Construct(TSharedPtr<FDetailTreeNode> DetailNode)
	{
		DetailNodePtr = DetailNode;
	}

	bool SDetailTreeItem::HasSplitter()
	{
		return false;
	}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION

}