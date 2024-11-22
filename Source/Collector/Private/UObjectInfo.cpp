//// Fill out your copyright notice in the Description page of Project Settings.
//#include "UObjectInfo.h"
//#include "UObjectHolder.h"
//#include "UObjectCollector.h"
//#include "Dom/JsonObject.h"
//#include "Serialization/JsonWriter.h"
//#include "Serialization/JsonSerializer.h"
//
//namespace UObjectCollector
//{
//	FUObjectInfo::FUObjectInfo(UObject* InObjectPtr)
//		:ObjectPtr(InObjectPtr)
//	{
//		if (InObjectPtr == nullptr) return;
//
//		Name = InObjectPtr->GetName();
//		ObjectIndex = InObjectPtr->GetUniqueID();
//		UClass* SuperClass = InObjectPtr->GetClass()->GetSuperClass();
//		SuperClassName = SuperClass ? SuperClass->GetName() : "";
//		ClassName = InObjectPtr->GetClass()->GetName();
//		ClassPath = InObjectPtr->GetClass()->GetPathName();
//		ObjectPath = InObjectPtr->GetPathName();
//
//		UObject* OuterObject = InObjectPtr->GetOuter();
//		if (OuterObject)
//		{
//			const int32 OuterIndex = OuterObject->GetUniqueID();
//			TSharedPtr<FUObjectHolder> OuterHolder = FUObjectCollector::Get().GetObject(OuterIndex);
//			if (OuterHolder->IsValid())
//			{
//				Outer = OuterHolder->GetObjectInfo();
//			}
//		}
//	}
//
//	FUObjectInfo::~FUObjectInfo()
//	{
//		if (Outer.IsValid())
//		{
//			Outer.Reset();
//			Outer = nullptr;
//		}
//
//		if (ObjectPtr.IsValid())
//		{
//			ObjectPtr.Reset();
//			ObjectPtr = nullptr;
//		}
//	}
//
//	bool FUObjectInfo::IsInModule(const FString& Module)
//	{
//		return ClassPath.StartsWith(Module);
//	}
//
//	FString FUObjectInfo::ToString()
//	{
//		TSharedPtr<FJsonObject> Json = ToJSON();
//		FString JsonString;
//		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
//		FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
//		return JsonString;
//	}
//
//	TSharedPtr<FJsonObject> FUObjectInfo::ToJSON()
//	{
//		TSharedPtr<FJsonObject> Result = MakeShareable(new FJsonObject());
//		Result->SetStringField(TEXT("Name"), Name);
//		Result->SetNumberField(TEXT("ObjectIndex"), ObjectIndex);
//		Result->SetStringField(TEXT("SuperClassName"), SuperClassName);
//		Result->SetStringField(TEXT("ClassName"), ClassName);
//		Result->SetStringField(TEXT("ClassPath"), ClassPath);
//		Result->SetStringField(TEXT("ObjectPath"), ObjectPath);
//		return Result;
//	}
//
//	void FUObjectInfo::FromJson(TSharedPtr<FJsonObject> JsonObject)
//	{
//		Name = JsonObject->GetStringField(TEXT("Name"));
//		ObjectIndex = JsonObject->GetNumberField(TEXT("ObjectIndex"));
//		SuperClassName = JsonObject->GetStringField(TEXT("SuperClassName"));
//		ClassName = JsonObject->GetStringField(TEXT("ClassName"));
//		ClassPath = JsonObject->GetStringField(TEXT("ClassPath"));
//		ObjectPath = JsonObject->GetStringField(TEXT("ObjectPath"));
//	}
//
//	void FUObjectInfo::OnSearch(const FText& Text)
//	{
//		if (!IsValid()) bVisible = false;
//
//		const FString& MyName = GetName();
//		bVisible = MyName.Contains(Text.ToString());
//	}
//
//	void FUObjectInfo::CloseSearch()
//	{
//		bVisible = true;
//	}
//}
