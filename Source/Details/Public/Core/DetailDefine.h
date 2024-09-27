// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailDefine.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogRuntimeDetail, Log, All);

#if ENGINE_MINOR_VERSION >= 25 || ENGINE_MAJOR_VERSION > 4
#define UE_Property FProperty
#else
#define UE_Property UProperty
#endif

/**
 *
 */
USTRUCT(BlueprintType)
struct DETAILSVIEWER_API FDetailOptions
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	bool bShowSerachBox;
};


namespace DETAILS_VIEWER
{

	enum EDetailNodeType
	{
		Category,
		ArrayElement,
		Array,
		MapElement,
		Map,
		Struct,
		Set,
		SetElement,
		NormalProperty,
		CustomNode
	};
}