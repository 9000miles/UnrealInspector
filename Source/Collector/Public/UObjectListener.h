// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/UObjectArray.h"

namespace UObjectCollector
{
	/**
	 * $Comment$
	 */
	class FUObjectListener : public FUObjectArray::FUObjectCreateListener, public FUObjectArray::FUObjectDeleteListener
	{
	public:
		FUObjectListener();
		~FUObjectListener();

		void NotifyUObjectCreated(const class UObjectBase* Object, int32 Index) override;
		void NotifyUObjectDeleted(const class UObjectBase* Object, int32 Index) override;
		void OnUObjectArrayShutdown() override;
	};
}