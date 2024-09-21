// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectHolder.h"
#include "ObjectInfo.h"
#include "Define.h"

namespace Inspector
{
	FObjectHolder::FObjectHolder(UObject* InObjectPtr) :
		ObjectPtr(InObjectPtr)
	{
		ObjectInfo = MakeShared<FObjectInfo>(InObjectPtr);
		UE_LOG(LogInspector, Log, TEXT("ObjectInfo: %s"), *ObjectInfo->ToString());
	}

	FObjectHolder::~FObjectHolder()
	{
		ObjectPtr.Reset();
		ObjectPtr = nullptr;
	}
}
