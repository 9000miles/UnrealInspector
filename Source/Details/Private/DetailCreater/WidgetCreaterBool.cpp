// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailCreater/WidgetCreaterBool.h"
#include "DetailWidget/SPropertyWidgetBool.h"
namespace DetailsViewer
{

	//TSharedPtr<SWidget> FWidgetCreaterBool::CreateWidget(TSharedPtr<FPropertyHolder> PropertyProxy)
	//{
	//	return SNew(SPropertyWidgetBool, PropertyProxy);
	//}

	TArray<FString> FWidgetCreaterBool::SupportTypes()
	{
		return { TEXT("bool"), TEXT("Bool"), TEXT("boolean") };
	}

}