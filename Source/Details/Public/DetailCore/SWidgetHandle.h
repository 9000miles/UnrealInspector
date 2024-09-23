// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWidgetHandle.generated.h"

USTRUCT(BlueprintType)
struct DETAILSVIEWER_API FSWidgetHandle
{
	GENERATED_BODY()
public:
	FSWidgetHandle() {}
	FSWidgetHandle(TSharedPtr<SWidget> InWidget)
	{
		Widget = InWidget;
	}

	TSharedPtr<SWidget> Widget = nullptr;
};
