// Fill out your copyright notice in the Description page of Project Settings.
#include "ScriptWidget.h"

#define LOCTEXT_NAMESPACE "FScriptWidgetModule"

void FScriptWidgetModule::StartupModule()
{
}

void FScriptWidgetModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FScriptWidgetModule, ScriptWidget);

void SCRIPT_WIDGET::$SButton::Construct(const FArguments& InArgs, FJsObject InJsObject)
{
	FArguments Args;

	JsObject = InJsObject;
	//TAttribute<FText>::FGetter Getter;
	//Getter.BindLambda([this]()
	//	{
	//		std::string text = JsObject.Get<std::string>("text");
	//		return FText::FromString(UTF8_TO_TCHAR(text.c_str()));
	//	});
	std::string text = JsObject.Get<std::string>("text");
	FText Text = FText::FromString(UTF8_TO_TCHAR(text.c_str()));
	Args._Text = TAttribute<FText>(Text);

	Args._OnClicked.BindLambda([this]()
		{
			//const FJsObject func = JsObject.Get<FJsObject>("onClick");
			//func.Action("onClick");
			UE_LOG(LogTemp, Warning, TEXT("callback: "));
			return FReply::Handled();
		})
		;

	SButton::Construct(Args);
}

FReply SCRIPT_WIDGET::$SButton::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SButton::OnMouseMove(MyGeometry, MouseEvent);

	/**
	 * 判断JsObject 是否包含onMouseMove函数，如果有则调用
	 */
	 //不需要回调
	//OnMouseMoveDelegate.ExecuteIfBound(MyGeometry, MouseEvent);
	return FReply::Handled();
}
