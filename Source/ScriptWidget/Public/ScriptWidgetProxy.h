// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScriptWidgetProxy.generated.h"

/**
 * $Comment$
 */
UCLASS(BlueprintType, Blueprintable)
class UScriptWidgetProxy : public UObject
{
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintCallable, Category = "ScriptWidget")
	//void SetWidget(FJsObject Widget)
	//{
	//	char* type = Widget.Get<char*>("__type_");
	//	UE_LOG(LogTemp, Warning, TEXT("type: %s"), type);
	//}

	TSharedPtr<SWidget> Widget;
};