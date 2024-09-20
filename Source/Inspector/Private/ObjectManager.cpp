// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectManager.h"
#include "Misc/LazySingleton.h"
#include "ObjectProxy.h"

namespace Inspector
{
	FObjectManager& FObjectManager::Get()
	{
		return TLazySingleton<FObjectManager>::Get();
	}

	void FObjectManager::AddObject(UObjectBase* ObjectBase, int32 Index)
	{
		if (!ObjectBase->GetClass()->IsChildOf(UObject::StaticClass())) return;

		UObject* Object = StaticCast<UObject*>(ObjectBase);
		FObjectProxy Proxy(Object);
		Objects.Add(Proxy);

		UE_LOG(LogTemp, Warning, TEXT("Inspector>> Add Object %s %d %d"), *Object->GetFName().ToString(), Object->GetUniqueID(), Index);
	}

	void FObjectManager::RemoveObject(UObjectBase* ObjectBase, int32 Index)
	{
		if (!ObjectBase->GetClass()->IsChildOf(UObject::StaticClass())) return;

		UObject* Object = StaticCast<UObject*>(ObjectBase);
		UE_LOG(LogTemp, Warning, TEXT("Inspector>> Remove Object %s %d %d"), *Object->GetFName().ToString(), Object->GetUniqueID(), Index);
	}

	void FObjectManager::Shutdown()
	{
		Objects.Empty();
	}

}
