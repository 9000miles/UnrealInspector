// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCreater.h"
#include "DetailWidget/SPropertyWidget.h"

namespace DetailsViewer
{
	class DETAILSVIEWER_API FWidgetCreaterBool : public FPropertyWidgetCreater
	{
	public:
		//TSharedPtr<SWidget> CreateWidget(TSharedPtr<FPropertyHolder> PropertyProxy) override;
		TArray<FString> SupportTypes() override;
	};
};
