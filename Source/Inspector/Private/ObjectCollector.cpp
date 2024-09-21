// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectCollector.h"
#include "Misc/LazySingleton.h"
#include "ObjectHolder.h"
#include "Define.h"
#include "ObjectInfo.h"

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

	TArray<TWeakPtr<FObjectHolder>> FObjectCollector::FindByName(const FString& Name)
	{
		TArray<TWeakPtr<FObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FObjectHolder> Holder = It.Value();
			if (Holder->GetObjectInfo()->GetName() == Name)
			{
				Result.Add(Holder);
			}
		}
		return Result;
	}

	TArray<TWeakPtr<FObjectHolder>> FObjectCollector::FindByClass(const FString& ClassName)
	{
		TArray<TWeakPtr<FObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FObjectHolder> Holder = It.Value();
			if (Holder->GetObjectInfo()->GetSuperClassName() == ClassName)
			{
				Result.Add(Holder);
			}
		}
		return Result;
	}

	TArray<TWeakPtr<FObjectHolder>> FObjectCollector::FindBySuperClass(const FString& ClassName)
	{
		TArray<TWeakPtr<FObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FObjectHolder> Holder = It.Value();
			if (Holder->GetObjectInfo()->GetClassName() == ClassName)
			{
				Result.Add(Holder);
			}
		}
		return Result;
	}

	TArray<TWeakPtr<FObjectHolder>> FObjectCollector::FindByOuter(const FString& Outer)
	{
		TArray<TWeakPtr<FObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FObjectHolder> Holder = It.Value();
			//if (Holder->GetObjectInfo()->GetName() == Name)
			//{
			//	Result.Add(Holder);
			//}
		}
		return Result;
	}

}
