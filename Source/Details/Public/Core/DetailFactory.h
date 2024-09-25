// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creater/DetailCreater.h"
namespace DetailsViewer
{

	class FDetailFactory
	{
	public:
		static FDetailFactory& Get();

		TSharedPtr<IDetailWidgetCreater> FindCreater(const FString& Identifier);
		void RegisterCreater(TSharedRef<IDetailWidgetCreater> Creater);

	private:
		TMap<FString, TSharedPtr<IDetailWidgetCreater>> Creaters;
	};
}