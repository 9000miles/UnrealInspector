// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectInfo.h"

namespace Inspector
{
	/**
	 * $Comment$
	 */
	class FObjectHolder
	{
	public:
		FObjectHolder(UObject* InObjectPtr);
		~FObjectHolder();

		inline bool IsValid() const { return ObjectPtr.IsValid(); }
		inline UObject* Get() const { return ObjectPtr.Get(); }
		inline void Reset() { ObjectPtr.Reset(); }
		inline TSharedRef<FObjectInfo> GetObjectInfo() { return ObjectInfo.ToSharedRef(); }

	private:
		TSharedPtr<FObjectInfo> ObjectInfo;
		TWeakObjectPtr<UObject> ObjectPtr;
	};
}