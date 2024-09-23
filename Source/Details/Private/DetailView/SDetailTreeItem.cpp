// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailView/SDetailTreeItem.h"
#include "SlateOptMacros.h"
#include "DetailView/SPropertyName.h"

namespace DetailsViewer
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