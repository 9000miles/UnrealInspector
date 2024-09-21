// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FJsonObject;

namespace Inspector
{
	class FObjectInfo : public TSharedFromThis<FObjectInfo>
	{
	public:
		FObjectInfo(UObject* InObjectPtr);
		~FObjectInfo();

		inline FString GetName() const { return Name; }
		inline int32 GetObjectIndex() const { return ObjectIndex; }
		inline FString GetSuperClassName() const { return SuperClassName; }
		inline FString GetClassName() const { return ClassName; }
		inline FString GetClassPath() const { return ClassPath; }
		inline FString GetObjectPath() const { return ObjectPath; }

		bool IsInModule(const FString& Module);

		FString ToString();
		TSharedPtr<FJsonObject> ToJSON();
		void FromJSON(TSharedPtr<FJsonObject> JsonObject);

	private:
		FString Name;
		int32 ObjectIndex;
		FString SuperClassName;
		FString ClassName;
		FString ClassPath;
		FString ObjectPath;

		TSharedPtr<FObjectInfo> Outer;
	};
}