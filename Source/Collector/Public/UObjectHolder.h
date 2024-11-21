// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjectInfo.h"

namespace UObjectCollector
{
	/**
	 * $Comment$
	 */
	class UOBJECTCOLLECTOR_API FUObjectHolder
	{
	public:
		FUObjectHolder(UObject* InObjectPtr);
		~FUObjectHolder();

		inline bool IsValid() const { return ObjectPtr.IsValid(); }
		inline UObject* Get() const { return ObjectPtr.Get(); }
		inline void Reset() { ObjectPtr.Reset(); }
		inline TSharedRef<FUObjectInfo> GetObjectInfo() { return ObjectInfo.ToSharedRef(); }

	private:
		TSharedPtr<FUObjectInfo> ObjectInfo;
		TWeakObjectPtr<UObject> ObjectPtr;
	};
}