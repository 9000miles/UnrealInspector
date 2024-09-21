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