// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectCollector.h"
#include "Misc/LazySingleton.h"
#include "ObjectHolder.h"
#include "Define.h"

namespace Inspector
{
	FObjectCollector& FObjectCollector::Get()
	{
		return TLazySingleton<FObjectCollector>::Get();
	}

	void FObjectCollector::AddObject(UObjectBase* ObjectBase, int32 Index)
	{
		if (!ObjectBase->GetClass()->IsChildOf(UObject::StaticClass())) return;

		UObject* Object = StaticCast<UObject*>(ObjectBase);
		TSharedPtr<FObjectHolder> Holder = MakeShared<FObjectHolder>(Object);
		ObjectHolders.Add(Index, Holder);

		UE_LOG(LogInspector, Warning, TEXT("Add Object %s %d %d"), *Object->GetFName().ToString(), Object->GetUniqueID(), Index);
	}

	void FObjectCollector::RemoveObject(UObjectBase* ObjectBase, int32 Index)
	{
		if (!ObjectBase->GetClass()->IsChildOf(UObject::StaticClass())) return;

		UObject* Object = StaticCast<UObject*>(ObjectBase);
		ObjectHolders[Index]->Reset();
		ObjectHolders.Remove(Index);

		UE_LOG(LogInspector, Warning, TEXT("Remove Object %s %d %d"), *Object->GetFName().ToString(), Object->GetUniqueID(), Index);
	}

	void FObjectCollector::Shutdown()
	{
		ObjectHolders.Empty();
	}

	TSharedPtr<FObjectHolder> FObjectCollector::GetObject(int32 Index)
	{
		if (!ObjectHolders.Contains(Index))
			return nullptr;

		return ObjectHolders[Index];
	}

	int32 FObjectCollector::GetObjectCount()
	{
		return ObjectHolders.Num();
	}
}
