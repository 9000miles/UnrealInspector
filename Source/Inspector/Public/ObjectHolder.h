// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace Inspector { class FObjectInfo; }

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
		inline const TSharedPtr<FObjectInfo>& GetObjectInfo() const { return ObjectInfo; }

	private:
		TSharedPtr<FObjectInfo> ObjectInfo;
		TWeakObjectPtr<UObject> ObjectPtr;
	};
}