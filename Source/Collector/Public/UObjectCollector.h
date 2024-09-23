// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjectHolder.h"

namespace UObjectCollector
{
	/**
	 * $Comment$
	 */
	class FUObjectCollector
	{
	public:
		static FUObjectCollector& Get();
		
		void AddObject(UObjectBase* Object, int32 Index);
		void RemoveObject(UObjectBase* Object, int32 Index);
		void Shutdown();

		TSharedPtr<FUObjectHolder> GetObject(int32 Index);
		int32 GetObjectCount();

		TArray<TWeakPtr<FUObjectHolder>> FindByName(const FString& Name);
		TArray<TWeakPtr<FUObjectHolder>> FindByClass(const FString& ClassName);
		TArray<TWeakPtr<FUObjectHolder>> FindBySuperClass(const FString& ClassName);
		TArray<TWeakPtr<FUObjectHolder>> FindByOuter(const FString& Outer);
		TArray<TWeakPtr<FUObjectHolder>> FindByModule(const FString& Module);

	private:
		TMap<int32, TSharedPtr<FUObjectHolder>> ObjectHolders;
	};
};