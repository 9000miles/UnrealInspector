// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectHolder.h"

namespace Inspector
{
	/**
	 * $Comment$
	 */
	class FObjectCollector
	{
	public:
		static FObjectCollector& Get();
		
		void AddObject(UObjectBase* Object, int32 Index);
		void RemoveObject(UObjectBase* Object, int32 Index);
		void Shutdown();

		TSharedPtr<FObjectHolder> GetObject(int32 Index);
		int32 GetObjectCount();

	private:
		TMap<int32, TSharedPtr<FObjectHolder>> ObjectHolders;
	};
};