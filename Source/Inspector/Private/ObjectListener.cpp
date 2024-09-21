// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectListener.h"
#include "ObjectCollector.h"
#include "UObject/UObjectArray.h"

namespace Inspector
{
	FObjectListener::FObjectListener()
	{
		GUObjectArray.AddUObjectCreateListener(this);
		GUObjectArray.AddUObjectDeleteListener(this);
	}

	FObjectListener::~FObjectListener()
	{
	}

	void FObjectListener::NotifyUObjectCreated(const class UObjectBase* Object, int32 Index)
	{
		FObjectCollector::Get().AddObject(const_cast<UObjectBase*>(Object), Index);
	}

	void FObjectListener::OnUObjectArrayShutdown()
	{
		GUObjectArray.RemoveUObjectDeleteListener(this);
		GUObjectArray.RemoveUObjectCreateListener(this);

		FObjectCollector::Get().Shutdown();
	}

	void FObjectListener::NotifyUObjectDeleted(const class UObjectBase* Object, int32 Index)
	{
		FObjectCollector::Get().RemoveObject(const_cast<UObjectBase*>(Object), Index);
	}
}
