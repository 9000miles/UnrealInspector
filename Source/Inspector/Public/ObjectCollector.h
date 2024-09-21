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

		TArray<TWeakPtr<FObjectHolder>> FindByName(const FString& Name);
		TArray<TWeakPtr<FObjectHolder>> FindByClass(const FString& ClassName);
		TArray<TWeakPtr<FObjectHolder>> FindBySuperClass(const FString& ClassName);
		TArray<TWeakPtr<FObjectHolder>> FindByOuter(const FString& Outer);
		TArray<TWeakPtr<FObjectHolder>> FindByModule(const FString& Module);

	private:
		TMap<int32, TSharedPtr<FObjectHolder>> ObjectHolders;
	};
};