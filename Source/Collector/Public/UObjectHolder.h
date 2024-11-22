// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjectInfo.h"

namespace UObjectCollector
{
	/**
	 * $Comment$
	 */
	class UOBJECTCOLLECTOR_API FUObjectHolder : public TSharedFromThis<FUObjectHolder>
	{
	public:
		FUObjectHolder(UObject* InObjectPtr);
		~FUObjectHolder();

		inline bool IsValid() const { return ObjectPtr.IsValid(); }
		inline UObject* Get() const { return ObjectPtr.Get(); }
		inline void Reset() { ObjectPtr.Reset(); ObjectPtr = nullptr; }

	public:
		inline FString GetName() const { return Name; }
		inline int32 GetObjectIndex() const { return ObjectIndex; }
		inline FString GetSuperClassName() const { return SuperClassName; }
		inline FString GetClassName() const { return ClassName; }
		inline FString GetClassPath() const { return ClassPath; }
		inline FString GetObjectPath() const { return ObjectPath; }
		inline TWeakObjectPtr<UObject> GetObject() const { return ObjectPtr; }
		inline UObject* GetUObject() const { return ObjectPtr.Get(); }
		inline TArray<TSharedPtr<FUObjectHolder>>& GetChildren() { return Children; }

		inline bool IsNull() const { return ObjectPtr.IsValid() == false; }
		inline bool IsValidIndex() const { return ObjectIndex != INDEX_NONE; }
		inline bool IsNullIndex() const { return ObjectIndex == INDEX_NONE; }

		inline bool operator==(const FUObjectHolder& Other) const
		{
			return ObjectPtr == Other.ObjectPtr;
		}

		inline bool operator!=(const FUObjectHolder& Other) const
		{
			return ObjectPtr != Other.ObjectPtr;
		}

		bool IsInModule(const FString& Module);
		void Sort();

		FString ToString();
		TSharedPtr<FJsonObject> ToJSON();
		void FromJson(TSharedPtr<FJsonObject> JsonObject);


		void OnSearch(const FText& Text);
		void CloseSearch();
		inline bool IsVisible() const { return bVisible; }

	private:
		bool bVisible;
		FString Name;
		int32 ObjectIndex;
		FString SuperClassName;
		FString ClassName;
		FString ClassPath;
		FString ObjectPath;

		TWeakObjectPtr<UObject> ObjectPtr;
		TSharedPtr<FUObjectHolder> Parent;
		TArray<TSharedPtr<FUObjectHolder>> Children;
	};
}