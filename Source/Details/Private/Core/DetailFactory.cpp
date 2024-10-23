// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/DetailFactory.h"
#include "Creater/DetailCreater.h"
#include "Misc/LazySingleton.h"

namespace DETAILS_VIEWER
{

	FWidgetCreaterFactory& FWidgetCreaterFactory::Get()
	{
		return TLazySingleton<FWidgetCreaterFactory>::Get();
	}

	TSharedPtr<IDetailWidgetCreater> FWidgetCreaterFactory::FindCreater(const FString& Identifier)
	{
		const bool bHasCreater = Creaters.Contains(Identifier);
		if (!bHasCreater)
		{
			UE_LOG(LogRuntimeDetail, Warning, TEXT("Not found %s UPropertyBuilder"), *Identifier);
			return nullptr;
		}

		TSharedPtr<IDetailWidgetCreater> CreaterPtr = Creaters[Identifier];
		return CreaterPtr;
	}

	void FWidgetCreaterFactory::RegisterCreater(TSharedRef<IDetailWidgetCreater> Creater)
	{
		const TArray<FString> Types = Creater->SupportTypes();
		for (const FString& PropertyType : Types)
		{
			const bool bHasCreater = Creaters.Contains(PropertyType);
			if (bHasCreater)
			{
				UE_LOG(LogRuntimeDetail, Warning, TEXT("%s multiple registration"), *PropertyType);
				continue;
			}

			Creaters.Add(PropertyType, Creater);
		}

	}

}