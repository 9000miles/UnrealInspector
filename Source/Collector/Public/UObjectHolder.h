// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObjectInfo.h"

namespace UOBJECT_COLLECTOR
{
	enum class EObjectSearchType :uint8
	{
		Name,
		Path,
		Class,
		Package,
		Module,
		Function,
	};

	struct UOBJECTCOLLECTOR_API FObjectSearchType
	{
		FObjectSearchType(EObjectSearchType InSearchType)
			:SearchType(InSearchType)
		{
		}

		FText GetText() const;
		EObjectSearchType SearchType;
	};

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
		inline void ClearChildren() { Children.Empty(); }

		inline bool IsNull() const { return ObjectPtr.IsValid() == false; }
		inline bool IsValidIndex() const { return ObjectIndex != INDEX_NONE; }
		inline bool IsNullIndex() const { return ObjectIndex == INDEX_NONE; }

		inline bool operator==(const FUObjectHolder& Other) const { return ObjectPtr == Other.ObjectPtr; }
		inline bool operator!=(const FUObjectHolder& Other) const { return ObjectPtr != Other.ObjectPtr; }

		bool IsInModule(const FString& Module);
		void Sort();

		FString ToString();
		TSharedPtr<FJsonObject> ToJSON();
		void FromJson(TSharedPtr<FJsonObject> JsonObject);

		void GetChildren(TArray<TSharedPtr<FUObjectHolder>>& OutChildren);
		bool Contains(TSharedPtr<FUObjectHolder>& Child);
		void AddChild(TSharedPtr<FUObjectHolder>& Child);

		void OnSearch(const FText& Text, EObjectSearchType SearchType);
		bool HasFunction(const FText& Text);
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
		TWeakPtr<FUObjectHolder> Parent;
		TArray<TWeakPtr<FUObjectHolder>> Children;
	};

	class UOBJECTCOLLECTOR_API FFunctionHolder : public TSharedFromThis<FFunctionHolder>
	{
	public:
		FString Name;
		UFunction* Function;
		TWeakObjectPtr<UObject> ObjectPtr;

		FFunctionHolder(UFunction* InFunction, TWeakObjectPtr<UObject> InObject)
			:Function(InFunction),
			ObjectPtr(InObject)
		{
			Name = InFunction->GetName();
		}

		FText GetFunctionName();
		FText GetFunctionSignature();
		int32 GetPropertiesSize() { return Function->GetPropertiesSize(); }
		void Invoke(uint8* Parameters);

		bool HasReturn();

	private:
		FString GetReturn();
		FString GetParameters();
		FString ParameterToString(FProperty* Parameter);
	};
}