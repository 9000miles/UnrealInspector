// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SPropertyName.h"
#include "SlateOptMacros.h"
namespace DETAILS_VIEWER
{
	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
		void SPropertyName::Construct(const FArguments& InArgs, TSharedPtr<FTreeNode> InTreeNode)
	{
		TreeNode = InTreeNode;

		const FString Name = GetDisplayName();

		ChildSlot
			[
				SNew(STextBlock)
					.Text(FText::FromString(Name))
			];

	}

	FString SPropertyName::GetDisplayName()
	{
		const FString Result = TreeNode->GetDisplayName();
		return Result.IsEmpty() ? TreeNode->GetName() : Result;
	}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION
}