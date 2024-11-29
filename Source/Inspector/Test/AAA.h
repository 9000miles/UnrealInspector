// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AAA.generated.h"

/**
 * $Comment$
 */
UCLASS(BlueprintType, Blueprintable)
class UAAA : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	int32 Test1(int32 a, int32 b = 2, bool c = true)
	{
		int32 d = a + b;
		UE_LOG(LogTemp, Warning, TEXT("ggggggggg  %d"), d);
		return d;
	}
	UFUNCTION(BlueprintCallable)
	void Test2(int32 a, int32 b, int32& c)
	{
		c = a + b;
	}
};