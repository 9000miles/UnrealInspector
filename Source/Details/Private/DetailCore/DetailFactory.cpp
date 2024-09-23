// Fill out your copyright notice in the Description page of Project Settings.

#include "DetailCore/DetailFactory.h"
#include "DetailCreater/DetailCreater.h"
#include "Misc/LazySingleton.h"

namespace DetailsViewer
{

	FDetailFactory& FDetailFactory::Get()
	{
		return TLazySingleton<FDetailFactory>::Get();
	}

	TSharedPtr<IDetailWidgetCreater> FDetailFactory::FindCreater(const FString& Identifier)
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

	void FDetailFactory::RegisterCreater(TSharedRef<IDetailWidgetCreater> Creater)
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