// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JsObject.h"
#include "ScriptWidgetFunction.generated.h"

/**
 * $Comment$
 */
UCLASS()
class UScriptWidgetFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
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
	//static void SetWidgetToWindow(UScriptWidgetProxy* WidgetProxy);

	static void createWidget(FString Type, FJsObject JsObject)
	{
		TSharedPtr<SWidget> Widget;
		if (Type == TEXT("Button"))
		{
			Widget = SNew(SButton)
				.Text_Lambda([JsObject]()
					{
						std::string text = JsObject.Get<std::string>("text");
						return FText::FromString(UTF8_TO_TCHAR(text.c_str()));
					})
				.OnClicked_Lambda([JsObject]()
					{
						//const FJsObject func = JsObject.Get<FJsObject>("onClick");
						//func.Action("onClick");
						UE_LOG(LogTemp, Warning, TEXT("callback: "));
						return FReply::Handled();
					})
				;
		}

		GEngine->GameViewport->AddViewportWidgetContent(Widget.ToSharedRef());
	}
};