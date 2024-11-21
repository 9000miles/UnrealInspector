// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FJsonObject;

namespace UObjectCollector
{
	class UOBJECTCOLLECTOR_API FUObjectInfo : public TSharedFromThis<FUObjectInfo>
	{
	public:
		FUObjectInfo(UObject* InObjectPtr);
		~FUObjectInfo();

		inline FString GetName() const { return Name; }
		inline int32 GetObjectIndex() const { return ObjectIndex; }
		inline FString GetSuperClassName() const { return SuperClassName; }
		inline FString GetClassName() const { return ClassName; }
		inline FString GetClassPath() const { return ClassPath; }
		inline FString GetObjectPath() const { return ObjectPath; }
		inline TWeakObjectPtr<UObject> GetObject() const { return ObjectPtr; }
		inline UObject* GetUObject() const { return ObjectPtr.Get(); }

		inline bool IsValid() const { return ObjectPtr.IsValid(); }
		inline bool IsNull() const { return ObjectPtr.IsValid() == false; }
		inline bool IsValidIndex() const { return ObjectIndex != INDEX_NONE; }
		inline bool IsNullIndex() const { return ObjectIndex == INDEX_NONE; }

		inline bool operator==(const FUObjectInfo& Other) const
		{
			return ObjectPtr == Other.ObjectPtr;
		}

		inline bool operator!=(const FUObjectInfo& Other) const
		{
			return ObjectPtr != Other.ObjectPtr;
		}

		bool IsInModule(const FString& Module);

		FString ToString();
		TSharedPtr<FJsonObject> ToJSON();
		void FromJson(TSharedPtr<FJsonObject> JsonObject);

	private:
		FString Name;
		int32 ObjectIndex;
		FString SuperClassName;
		FString ClassName;
		FString ClassPath;
		FString ObjectPath;

		TWeakObjectPtr<UObject> ObjectPtr;
		TSharedPtr<FUObjectInfo> Outer;
	};
}