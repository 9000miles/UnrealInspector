// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailCreater/WidgetCreaterString.h"
#include "DetailWidget/SPropertyWidget.h"
#include "DetailWidget/SPropertyWidgetString.h"

namespace DetailsViewer
{


	FWidgetCreaterString::FWidgetCreaterString()
	{
	}

	//TSharedPtr<SPropertyWidget> FWidgetCreaterString::CreateWidget(TSharedPtr<FPropertyHolder> PropertyProxy)
	//{
	//	return SNew(SPropertyWidgetString, PropertyProxy);
	//}

	TArray<FString> FWidgetCreaterString::SupportTypes()
	{
		return { TEXT("FString") ,TEXT("string"), TEXT("FName"), TEXT("FText") };
	}

}