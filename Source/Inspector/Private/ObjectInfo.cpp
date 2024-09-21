// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectInfo.h"
#include "ObjectHolder.h"
#include "ObjectCollector.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

namespace Inspector
{
	FObjectInfo::FObjectInfo(UObject* InObjectPtr)
	{
		if (InObjectPtr == nullptr) return;

		Name = InObjectPtr->GetName();
		ObjectIndex = InObjectPtr->GetUniqueID();
		UClass* SuperClass = InObjectPtr->GetClass()->GetSuperClass();
		SuperClassName = SuperClass ? SuperClass->GetName() : "";
		ClassName = InObjectPtr->GetClass()->GetName();
		ClassPath = InObjectPtr->GetClass()->GetPathName();
		ObjectPath = InObjectPtr->GetPathName();

		UObject* OuterObject = InObjectPtr->GetOuter();
		if (OuterObject)
		{
			const int32 OuterIndex = OuterObject->GetUniqueID();
			TSharedPtr<FObjectHolder> OuterHolder = FObjectCollector::Get().GetObject(OuterIndex);
			if (OuterHolder->IsValid())
			{
				Outer = OuterHolder->GetObjectInfo();
			}
		}
	}

	FObjectInfo::~FObjectInfo()
	{
		if (Outer.IsValid())
		{
			Outer.Reset();
			Outer = nullptr;
		}
	}

	bool FObjectInfo::IsInModule(const FString& Module)
	{
		return ClassPath.StartsWith(Module);
	}

	FString FObjectInfo::ToString()
	{
		TSharedPtr<FJsonObject> Json = ToJSON();
		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
		return JsonString;
	}

	TSharedPtr<FJsonObject> FObjectInfo::ToJSON()
	{
		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
		Result->SetStringField(TEXT("Name"), Name);
		Result->SetNumberField(TEXT("ObjectIndex"), ObjectIndex);
		Result->SetStringField(TEXT("SuperClassName"), SuperClassName);
		Result->SetStringField(TEXT("ClassName"), ClassName);
		Result->SetStringField(TEXT("ClassPath"), ClassPath);
		Result->SetStringField(TEXT("ObjectPath"), ObjectPath);
		return Result;
	}

	void FObjectInfo::FromJSON(TSharedPtr<FJsonObject> JsonObject)
	{
		Name = JsonObject->GetStringField(TEXT("Name"));
		ObjectIndex = JsonObject->GetNumberField(TEXT("ObjectIndex"));
		SuperClassName = JsonObject->GetStringField(TEXT("SuperClassName"));
		ClassName = JsonObject->GetStringField(TEXT("ClassName"));
		ClassPath = JsonObject->GetStringField(TEXT("ClassPath"));
		ObjectPath = JsonObject->GetStringField(TEXT("ObjectPath"));
	}
}
