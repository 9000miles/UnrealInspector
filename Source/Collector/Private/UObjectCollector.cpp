// Fill out your copyright notice in the Description page of Project Settings.
#include "UObjectCollector.h"
#include "Misc/LazySingleton.h"
#include "UObjectHolder.h"
#include "Define.h"
#include "UObjectInfo.h"

namespace UObjectCollector
{

	UObjectCollector::FUObjectEvent FUObjectCollector::OnObjectAddEvent;

	UObjectCollector::FUObjectEvent FUObjectCollector::OnObjectDeleteEvent;

	FUObjectCollector& FUObjectCollector::Get()
	{
		return TLazySingleton<FUObjectCollector>::Get();
	}

	void FUObjectCollector::AddObject(UObjectBase* ObjectBase, int32 Index)
	{
		if (!ObjectBase->GetClass()->IsChildOf(UObject::StaticClass())) return;

		UObject* Object = StaticCast<UObject*>(ObjectBase);
		TSharedPtr<FUObjectHolder> Holder = MakeShared<FUObjectHolder>(Object);
		ObjectHolders.Add(Index, Holder);

		OnObjectAddEvent.Broadcast(Holder);
	}

	void FUObjectCollector::RemoveObject(UObjectBase* ObjectBase, int32 Index)
	{
		if (!ObjectBase->GetClass()->IsChildOf(UObject::StaticClass())) return;

		UObject* Object = StaticCast<UObject*>(ObjectBase);
		TSharedPtr<FUObjectHolder> Holder = ObjectHolders[Index];
		Holder->Reset();
		ObjectHolders.Remove(Index);

		OnObjectDeleteEvent.Broadcast(Holder);
	}

	void FUObjectCollector::Shutdown()
	{
		ObjectHolders.Empty();
	}

	TSharedPtr<FUObjectHolder> FUObjectCollector::GetObject(int32 Index)
	{
		if (!ObjectHolders.Contains(Index))
			return nullptr;

		return ObjectHolders[Index];
	}

	int32 FUObjectCollector::GetObjectCount()
	{
		return ObjectHolders.Num();
	}

	TArray<TWeakPtr<FUObjectHolder>> FUObjectCollector::FindByName(const FString& Name)
	{
		TArray<TWeakPtr<FUObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FUObjectHolder> Holder = It.Value();
			if (Holder->GetObjectInfo()->GetName() == Name)
			{
				Result.Add(Holder);
			}
		}
		return Result;
	}

	TArray<TWeakPtr<FUObjectHolder>> FUObjectCollector::FindByClass(const FString& ClassName)
	{
		TArray<TWeakPtr<FUObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FUObjectHolder> Holder = It.Value();
			if (Holder->GetObjectInfo()->GetSuperClassName() == ClassName)
			{
				Result.Add(Holder);
			}
		}
		return Result;
	}

	TArray<TWeakPtr<FUObjectHolder>> FUObjectCollector::FindBySuperClass(const FString& ClassName)
	{
		TArray<TWeakPtr<FUObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FUObjectHolder> Holder = It.Value();
			if (Holder->GetObjectInfo()->GetClassName() == ClassName)
			{
				Result.Add(Holder);
			}
		}
		return Result;
	}

	TArray<TWeakPtr<FUObjectHolder>> FUObjectCollector::FindByOuter(const FString& Outer)
	{
		TArray<TWeakPtr<FUObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FUObjectHolder> Holder = It.Value();
			//if (Holder->GetObjectInfo()->GetName() == Name)
			//{
			//	Result.Add(Holder);
			//}
		}
		return Result;
	}

	TArray<TWeakPtr<FUObjectHolder>> FUObjectCollector::FindByModule(const FString& Module)
	{
		TArray<TWeakPtr<FUObjectHolder>> Result;
		for (auto It = ObjectHolders.CreateIterator(); It; ++It)
		{
			TSharedPtr<FUObjectHolder> Holder = It.Value();
			if (Holder->GetObjectInfo()->IsInModule(Module))
			{
				Result.Add(Holder);
			}
		}
		return Result;
	}

	void FUObjectCollector::GetAll(TArray<TSharedPtr<FUObjectHolder>>& Out)
	{
		ObjectHolders.GenerateValueArray(Out);
	}
}
