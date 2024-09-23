// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DetailWidget/SPropertyWidget.h"
#include "DetailCore/PropertyHolder.h"

namespace DetailsViewer
{
	/**
	 * 细节面板创建器
	 *	整个面板创建
	 */
	class IDetailWidgetCreater
	{
	public:
		virtual TSharedPtr<SWidget> CreateWidget() = 0;
		virtual TArray<FString> SupportTypes() = 0;
	};

	/**
	 * 细节面板整行创建器
	 */
	class FRowWidgetCreater :public IDetailWidgetCreater
	{
	public:
		virtual TSharedPtr<SWidget> CreateRowWidget(TSharedPtr<FPropertyHolder> PropertyHolder);

	private:
		TSharedPtr<SWidget> CreateWidget() override;
		TArray<FString> SupportTypes() override;

	};

	/**
	 * 属性创建器，包含属性名称
	 */
	class FPropertyWidgetCreater :public FRowWidgetCreater
	{
	public:
		TSharedPtr<SWidget> CreateRowWidget(TSharedPtr<FPropertyHolder> PropertyHolder) override;
		virtual TArray<FString> SupportTypes() override;
		virtual bool IsShowReset();
		virtual bool OverrideRowWidget();


	private:
		TSharedPtr<SWidget> CreateWidget() override;

	};

}