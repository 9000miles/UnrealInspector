// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailDefine.h"
#include "DetailViewer.h"
#include "Creater/DetailCreater.h"
#include "DetailInfo.h"

using namespace DETAILS_VIEWER;

/**
 *
 */
class DETAILSVIEWER_API FDetialManager 
{
public:
	FDetialManager();
	~FDetialManager();

	static FDetialManager& Get();

	template<typename T>
	TSharedPtr<T> Create(TSharedPtr<FDetailOptions> Options);
};

template<typename T>
TSharedPtr<T> FDetialManager::Create(TSharedPtr<FDetailOptions> Options)
{
	TSharedPtr<T> Instance = MakeShared<T>();
	//TSharedPtr<T> Instance = Factory::Get<T>(T::TypeName());
	check(Instance.IsValid());

	Instance->Init(Options);

	return Instance;
}
