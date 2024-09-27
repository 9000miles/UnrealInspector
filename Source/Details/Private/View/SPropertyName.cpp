// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SPropertyName.h"
#include "SlateOptMacros.h"
namespace DETAILS_VIEWER
{
	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
		void SPropertyName::Construct(const FArguments& InArgs, TSharedPtr<FDetailTreeNode> DetailNode)
	{
		const FString Name = DetailNode->GetName();

		ChildSlot
			[
				SNew(STextBlock)
					.Text(FText::FromString(Name))
			];

	}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION
}