// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SDetailCategory.h"
#include "SlateOptMacros.h"
#include "Styling/AppStyle.h"

namespace DETAILS_VIEWER
{

	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
		void SDetailCategory::Construct(const FArguments& InArgs, TSharedPtr<FTreeNode> InTreeNode)
	{
		SDetailTreeItem::Construct(InTreeNode);
		//SDetailTreeItem::Construct(StaticCastSharedPtr<FCategoryTreeNode>(InTreeNode));

		ChildSlot
			[
				//SNew(SExpandableArea)
				//	.AreaTitle(FText::FromString(CategoryNode->GetName()))
				//	.HeaderContent()
				//	[
				SNew(STextBlock)
					.Text(FText::FromString(NodePtr->GetName()))
					.TextStyle(FAppStyle::Get(), "ButtonText")
					.Font(FAppStyle::Get().GetFontStyle("NormalFontBold"))
					//]
					//.BodyContent()
					//[
					//]
			];
	}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION

		bool SDetailCategory::HasSplitter()
	{
		return false;
	}

}