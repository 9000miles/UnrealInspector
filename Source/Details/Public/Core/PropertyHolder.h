// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailDefine.h"
#include "PropertyHolder.generated.h"

USTRUCT(BlueprintType)
struct FPropertyHandle
{
	GENERATED_BODY()
public:
	FPropertyHandle() {}
	FPropertyHandle(UE_Property* InProperty)
	{
		Property = InProperty;
	}
	UE_Property* Property;
};

/**
 *
 */
//class DETAILSVIEWER_API FPropertyHolder
//{
//public:
//	FPropertyHolder(TWeakObjectPtr<UObject> Object, UE_Property* Property);
//	~FPropertyHolder();
//
//	const FString GetName();
//	const FText GetDisplayName();
//	const FString GetPropertyType();
//	UE_Property* GetProperty() { return MyProperty; }
//	UObject* GetOutermost() { return Outermost.Get(); }
//	const bool IsA(FFieldClass* InClass) { return MyProperty->IsA(InClass); }
//
//private:
//	UE_Property* MyProperty;
//	TWeakObjectPtr<UObject> Outermost;
//};
