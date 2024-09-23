// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCreater.h"
#include "DetailCore/PropertyHolder.h"

class SPropertyWidgetString;
namespace DetailsViewer
{
	/**
	 *
	 */
	class DETAILSVIEWER_API FWidgetCreaterString : public FPropertyWidgetCreater
	{
	public:
		FWidgetCreaterString();
		//TSharedPtr<SPropertyWidget> CreateWidget(TSharedPtr<FPropertyHolder> PropertyProxy) override;
		TArray<FString> SupportTypes() override;

	private:
	};

};