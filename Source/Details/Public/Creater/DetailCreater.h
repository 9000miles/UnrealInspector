// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Widget/SPropertyWidget.h"
#include "Core/PropertyHolder.h"
#include "Core/DetailInfo.h"

namespace DETAILS_VIEWER
{
	/**
	 * 细节面板创建器
	 *	整个面板创建
	 */
	class IDetailWidgetCreater :public ITypeName
	{
	public:
		virtual ~IDetailWidgetCreater() {}
		virtual TSharedPtr<SWidget> MakeWidget() = 0;
		virtual TArray<FString> SupportTypes() = 0;
	};

	/**
	 * 细节面板整行创建器
	 */
	class FRowWidgetCreater :public IDetailWidgetCreater
	{
	public:
		virtual ~FRowWidgetCreater() {}
		virtual TSharedPtr<SWidget> CreateRowWidget(TSharedPtr<FPropertyHolder> PropertyHolder);

	private:
		TSharedPtr<SWidget> MakeWidget() override;
		TArray<FString> SupportTypes() override;

	};

	/**
	 * 属性创建器，包含属性名称
	 */
	class FPropertyWidgetCreater :public FRowWidgetCreater
	{
		friend class FNormalNode;
	public:
		virtual ~FPropertyWidgetCreater() {}
	public:
		virtual TArray<FString> SupportTypes() override;
		virtual bool IsShowReset();
		virtual bool OverrideRowWidget();


	private:
		TSharedPtr<SWidget> MakeWidget() override;
	protected:
		TSharedPtr<FPropertyHolder> PropertyHolder;
	};

}