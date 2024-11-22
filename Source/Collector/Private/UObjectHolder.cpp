// Fill out your copyright notice in the Description page of Project Settings.
#include "UObjectHolder.h"
#include "UObjectInfo.h"
#include "Define.h"
#include "UObjectCollector.h"

namespace UObjectCollector
{
	FUObjectHolder::FUObjectHolder(UObject* InObjectPtr) :
		ObjectPtr(InObjectPtr)
	{
		//ObjectInfo = MakeShared<FUObjectInfo>(InObjectPtr);
		//UE_LOG(LogCollector, Log, TEXT("ObjectInfo: %s"), *ObjectInfo->ToString());

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
			TSharedPtr<FUObjectHolder> OuterHolder = FUObjectCollector::Get().GetObject(OuterIndex);
			if (OuterHolder->IsValid())
			{
				//Outer = OuterHolder->GetObjectInfo();
			}
		}
	}

	FUObjectHolder::~FUObjectHolder()
	{
		ObjectPtr.Reset();
		ObjectPtr = nullptr;
	}

	bool FUObjectHolder::IsInModule(const FString& Module)
	{
		return ClassPath.StartsWith(Module);
	}

	FString FUObjectHolder::ToString()
	{
		TSharedPtr<FJsonObject> Json = ToJSON();
		FString JsonString;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
		return JsonString;
	}

	TSharedPtr<FJsonObject> FUObjectHolder::ToJSON()
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

	void FUObjectHolder::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
		Name = JsonObject->GetStringField(TEXT("Name"));
		ObjectIndex = JsonObject->GetNumberField(TEXT("ObjectIndex"));
		SuperClassName = JsonObject->GetStringField(TEXT("SuperClassName"));
		ClassName = JsonObject->GetStringField(TEXT("ClassName"));
		ClassPath = JsonObject->GetStringField(TEXT("ClassPath"));
		ObjectPath = JsonObject->GetStringField(TEXT("ObjectPath"));
	}

	void FUObjectHolder::OnSearch(const FText& Text)
	{
		if (!IsValid()) bVisible = false;

		const FString& MyName = GetName();
		bVisible = MyName.Contains(Text.ToString());
	}

	void FUObjectHolder::CloseSearch()
	{
		bVisible = true;
	}

}
