// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Widget/SPropertyWidget.h"
#include "Core/PropertyHolder.h"
#include "Core/DetailInfo.h"
#include <Node\DetailNode.h>

namespace DETAILS_VIEWER
{
	/**
	 * 细节面板创建器
	 *	整个面板创建
	 */
	class IDetailWidgetCreater :public TSharedFromThis<IDetailWidgetCreater>, public ITypeName
	{
	public:
		IDetailWidgetCreater();
		virtual ~IDetailWidgetCreater() {}
		virtual TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) = 0;
		virtual TArray<FString> SupportTypes() = 0;

		FString GetTypeName() override;
		void Initalized() override;
		virtual bool IsAllowHasChildren();
	};

	/**
	 * 细节面板整行创建器
	 */
	class FRowWidgetCreater :public IDetailWidgetCreater
	{
	public:
		virtual ~FRowWidgetCreater() {}
		virtual TSharedPtr<SWidget> CreateRowWidget(TSharedPtr<FTreeNode> TreeNode);

	private:
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;
		TArray<FString> SupportTypes() override;

	};

	/**
	 * 属性创建器，包含属性名称
	 */
	class FPropertyWidgetCreater :public FRowWidgetCreater
	{
		friend class FGeneralNode;
	public:
		FPropertyWidgetCreater();
		virtual ~FPropertyWidgetCreater() {}
	public:
		virtual TArray<FString> SupportTypes() override;
		virtual bool IsShowReset();
		virtual bool OverrideRowWidget();

		//virtual bool IsSupport(TSharedPtr<FPropertyInfo> PropertyInfo);


	private:
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;

	protected:
		template<typename T>
		TSharedPtr<SWidget> MakePropertyWidget(TSharedPtr<FTreeNode> TreeNode)
		{
			TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
			return SNew(T, PropertyTreeNode->PropertyInfo);
		}
	protected:
		TSharedPtr<PROPERTY::IExecutor> Executor;
	};

}