// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailDefine.h"
#include "DetailViewer.h"
#include "Creater/DetailCreater.h"
#include "DetialManger.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class DETAILSVIEWER_API UDetialManager :public UObject
{
	GENERATED_BODY()
public:
	UDetialManager();
	~UDetialManager();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetDetailManger"))
	static UDetialManager* Get();

	UFUNCTION(BlueprintCallable)
	UDetailViewer* CreateDetail(FDetailOptions Options);

private:
	static TStrongObjectPtr<UDetialManager> Instance;
};
