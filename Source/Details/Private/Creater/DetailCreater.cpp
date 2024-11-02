// Fill out your copyright notice in the Description page of Project Settings.


#include "Creater/DetailCreater.h"
namespace DETAILS_VIEWER
{



	FPropertyWidgetCreater::FPropertyWidgetCreater()
	{
	}

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


	//bool FPropertyWidgetCreater::IsSupport(TSharedPtr<FPropertyInfo> PropertyInfo)
	//{
	//	const TArray<FString> Typies = SupportTypes();
	//	return Typies.ContainsByPredicate([PropertyInfo](const FString& Type) { return Type == PropertyInfo->Type; });
	//}

	TSharedPtr<SWidget> FPropertyWidgetCreater::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		return SNullWidget::NullWidget;

	}

	//TSharedPtr<SWidget> FPropertyWidgetCreater::CreateRowWidget(TSharedPtr<FPropertyHolder> PropertyHolder)
	//{
	//	return SNullWidget::NullWidget;

	//}

	TSharedPtr<SWidget> FRowWidgetCreater::CreateRowWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		return SNullWidget::NullWidget;
	}



	TSharedPtr<SWidget> FRowWidgetCreater::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		return SNullWidget::NullWidget;
	}


	TArray<FString> FRowWidgetCreater::SupportTypes()
	{
		return TArray<FString>();
	}


	IDetailWidgetCreater::IDetailWidgetCreater()
	{
	}


	FString IDetailWidgetCreater::GetTypeName()
	{
		return TEXT("IDetailWidgetCreater");
	}


	void IDetailWidgetCreater::Initalized()
	{
		FWidgetCreaterFactory::Get().RegisterCreater(AsShared());
	}


	bool IDetailWidgetCreater::IsAllowHasChildren()
	{
		return true;
	}

}