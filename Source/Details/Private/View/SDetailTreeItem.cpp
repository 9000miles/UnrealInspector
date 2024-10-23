// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SDetailTreeItem.h"
#include "SlateOptMacros.h"
#include "View/SPropertyName.h"

namespace DETAILS_VIEWER
{

	//bool SDetailTreeItem<T>::HasSplitter()
	//{
	//	return false;
	//}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION


		void SDetailTreeItem::Construct(TSharedPtr<FTreeNode> DetailNode)
	{


		NodePtr = DetailNode;
	}

}