// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsObject.h"
#include "ScriptWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "ScriptWidgetFunction.generated.h"

/**
 * $Comment$
 */
UCLASS()
class UScriptWidgetFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * FJsObject Widget需要改成 v8::FunctionCallbackInfo<v8::Value>& Info
	 * 纯C++函数
	 */
	UFUNCTION(BlueprintCallable, Category = "ScriptWidget")
	static void SetWidgetToWindow(FJsObject Widget)
	{
		char* type = Widget.Get<char*>("__type__");
		UE_LOG(LogTemp, Warning, TEXT("type: %s"), type);

		std::string type1 = Widget.Get<std::string>("__type__");

		//UE_LOG(LogTemp, Warning, TEXT("type: %s"), *FString(ANSI_TO_TCHAR(type1))); 
		const FString Type = FString(UTF8_TO_TCHAR(type1.c_str()));
		UE_LOG(LogTemp, Warning, TEXT("type: %s"), *Type);

		createWidget(Type, Widget);

	}
	/**
	 *
	 */
	 //static void SetWidgetToWindow(UScriptWidgetProxy* WidgetProxy);

	static void createWidget(FString Type, FJsObject JsObject)
	{
		/**
		 * 根据传过来的类名，创建对应的Widget
		 */
		TSharedPtr<SWidget> Widget;
		if (Type == TEXT("Button"))
		{
			Widget = SNew(SCRIPT_WIDGET::$SButton, JsObject)
				;
		}
		else if (Type == TEXT("TextBlock"))
		{
			//Widget = SNew(SCRIPT_WIDGET::$STextBlock, JsObject)
			//	;
		}

		GEngine->GameViewport->AddViewportWidgetContent(Widget.ToSharedRef());
	}
};

namespace SCRIPT_WIDGET
{
	//class $STextBlock : public STextBlock, public ITypeName
	//{
	//	IMPLEMENT_ITYPENAME(STextBlock)
	//public:
	//
	//public:
	//};
}