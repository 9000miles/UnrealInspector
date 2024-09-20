// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace Inspector
{
	/**
	 * $Comment$
	 */
	class FObjectProxy
	{
	public:
		FObjectProxy(UObject* InObjectPtr) :
			ObjectPtr(InObjectPtr)
		{
		}

		TWeakObjectPtr<UObject> ObjectPtr;
	};
}