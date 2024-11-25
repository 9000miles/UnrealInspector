// Fill out your copyright notice in the Description page of Project Settings.
#include "UObjectListener.h"
#include "UObjectCollector.h"
#include "UObject/UObjectArray.h"

namespace UOBJECT_COLLECTOR
{
	FUObjectListener::FUObjectListener()
	{
		GUObjectArray.AddUObjectCreateListener(this);
		GUObjectArray.AddUObjectDeleteListener(this);
	}

	FUObjectListener::~FUObjectListener()
	{
	}

	void FUObjectListener::NotifyUObjectCreated(const class UObjectBase* Object, int32 Index)
	{
		FUObjectCollector::Get().AddObject(const_cast<UObjectBase*>(Object), Index);
	}

	void FUObjectListener::OnUObjectArrayShutdown()
	{
		GUObjectArray.RemoveUObjectDeleteListener(this);
		GUObjectArray.RemoveUObjectCreateListener(this);

		FUObjectCollector::Get().Shutdown();
	}

	void FUObjectListener::NotifyUObjectDeleted(const class UObjectBase* Object, int32 Index)
	{
		FUObjectCollector::Get().RemoveObject(const_cast<UObjectBase*>(Object), Index);
	}
}
