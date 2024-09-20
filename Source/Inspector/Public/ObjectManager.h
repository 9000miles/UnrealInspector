// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectProxy.h"

namespace Inspector
{
	/**
	 * $Comment$
	 */
	class FObjectManager
	{
	public:
		static FObjectManager& Get();
		void AddObject(UObjectBase* Object, int32 Index);
		void RemoveObject(UObjectBase* Object, int32 Index);
		void Shutdown();

	private:
		TArray<FObjectProxy> Objects;
	};
};