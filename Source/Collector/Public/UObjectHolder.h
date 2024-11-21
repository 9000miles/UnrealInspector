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
		inline void Reset() { ObjectPtr.Reset(); ObjectPtr = nullptr; }
		inline TSharedRef<FUObjectInfo> GetObjectInfo() { return ObjectInfo.ToSharedRef(); }
		void OnSearch(const FText& Text);
		void CloseSearch();
		inline bool IsVisible() const { return bVisible; }

	private:
		bool bVisible;
		TSharedPtr<FUObjectInfo> ObjectInfo;
		TWeakObjectPtr<UObject> ObjectPtr;
	};
}