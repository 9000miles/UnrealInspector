// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "JsObject.h"

class FScriptWidgetModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

namespace SCRIPT_WIDGET
{
#define IMPLEMENT_ITYPENAME(ClassName) \
public: \
	FString GetTypeName() override { return ClassName::TypeName(); } \
	static FString TypeName() { return TEXT(#ClassName); }

	class ITypeName
	{
	public:
		virtual FString GetTypeName() = 0;
		virtual void Initalized() {}

		template<typename T>
		T CreateInstance() {}
	};

	/**
	 * 需要继承，这样可以实现虚函数在ts侧覆写
	 * 根据TypeName注册类
	 */
	class $SButton : public SButton, public ITypeName
	{
		IMPLEMENT_ITYPENAME($SButton)
	public:
		/**
		 * FJsObject参数后面需要改成 v8::FunctionCallbackInfo<v8::Value>& Info
		 */
		void Construct(const FArguments& InArgs, FJsObject InJsObject);
		FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	public:

		DECLARE_DELEGATE_TwoParams(FOnMouseMove, const FGeometry&, const FPointerEvent&);
		FOnMouseMove OnMouseMoveDelegate;

		FJsObject JsObject;
	};
}