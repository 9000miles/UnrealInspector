// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailFactory.h"
#include "DetailDefine.h"
#include "View/SDetailViewer.h"
#include "DetailViewer.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class DETAILSVIEWER_API UDetailViewer :public UObject
{
	GENERATED_BODY()
public:
	UDetailViewer();
	void InitByOptions(FDetailOptions Options);

	UFUNCTION(BlueprintCallable)
	void SetObject(UObject* Object);

	UFUNCTION(BlueprintPure)
	FGuid GetGuid() { return Guid; }

	TSharedPtr<DETAILS_VIEWER::SDetailViewer> GetViewWidget() { return DetailView; }

public:
	UPROPERTY(BlueprintReadWrite)
	bool bShowSerachBox;



private:
	FGuid Guid;
	TSharedPtr<DETAILS_VIEWER::SDetailViewer> DetailView;
};
