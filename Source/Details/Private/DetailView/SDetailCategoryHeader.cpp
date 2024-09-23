// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailView/SDetailCategoryHeader.h"
#include "SlateOptMacros.h"

namespace DetailsViewer
{

	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
		void SDetailCategoryHeader::Construct(const FArguments& InArgs)
	{
		/*
		ChildSlot
		[
			// Populate the widget
		];
		*/
	}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION

		bool SDetailCategoryHeader::HasSplitter()
	{
		return false;
	}

}