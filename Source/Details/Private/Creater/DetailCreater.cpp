// Fill out your copyright notice in the Description page of Project Settings.


#include "Creater/DetailCreater.h"
namespace DETAILS_VIEWER
{



	TArray<FString> FPropertyWidgetCreater::SupportTypes()
	{
		return TArray<FString>();
	}

	bool FPropertyWidgetCreater::IsShowReset()
	{
		return false;
	}

	bool FPropertyWidgetCreater::OverrideRowWidget()
	{
		return false;
	}


	TSharedPtr<SWidget> FPropertyWidgetCreater::MakeWidget()
	{
		return SNullWidget::NullWidget;

	}

	//TSharedPtr<SWidget> FPropertyWidgetCreater::CreateRowWidget(TSharedPtr<FPropertyHolder> PropertyHolder)
	//{
	//	return SNullWidget::NullWidget;

	//}

	TSharedPtr<SWidget> FRowWidgetCreater::CreateRowWidget(TSharedPtr<FPropertyHolder> PropertyHolder)
	{
		return SNullWidget::NullWidget;
	}



	TSharedPtr<SWidget> FRowWidgetCreater::MakeWidget()
	{
		return SNullWidget::NullWidget;
	}


	TArray<FString> FRowWidgetCreater::SupportTypes()
	{
		return TArray<FString>();
	}

}