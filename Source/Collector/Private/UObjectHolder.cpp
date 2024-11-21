// Fill out your copyright notice in the Description page of Project Settings.
#include "UObjectHolder.h"
#include "UObjectInfo.h"
#include "Define.h"

namespace UObjectCollector
{
	FUObjectHolder::FUObjectHolder(UObject* InObjectPtr) :
		ObjectPtr(InObjectPtr)
	{
		ObjectInfo = MakeShared<FUObjectInfo>(InObjectPtr);
		//UE_LOG(LogCollector, Log, TEXT("ObjectInfo: %s"), *ObjectInfo->ToString());
	}

	FUObjectHolder::~FUObjectHolder()
	{
		ObjectPtr.Reset();
		ObjectPtr = nullptr;
	}

	void FUObjectHolder::OnSearch(const FText& Text)
	{
		if (!ObjectInfo.IsValid()) bVisible = false;

		const FString& Name = ObjectInfo->GetName();
		bVisible = Name.Contains(Text.ToString());
	}

	void FUObjectHolder::CloseSearch()
	{
		bVisible = true;
	}

}
