// Fill out your copyright notice in the Description page of Project Settings.
#include "UObjectHolder.h"
#include "UObjectInfo.h"
#include "Define.h"
#include "UObjectCollector.h"

#define LOCTEXT_NAMESPACE "FUObjectHolder"

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


	void FUObjectHolder::Sort()
	{
		Children.Sort([](const TSharedPtr<FUObjectHolder>& A, const TSharedPtr<FUObjectHolder>& B)
			{
				return A->GetName() < B->GetName();
			});

		for (auto& Child : Children)
		{
			Child->Sort();
		}
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

	void FUObjectHolder::OnSearch(const FText& Text, EObjectSearchType SearchType)
	{
		if (!IsValid()) bVisible = false;

		switch (SearchType)
		{
		case EObjectSearchType::Name: bVisible = Name.Contains(Text.ToString()); break;
		case EObjectSearchType::Path: bVisible = ObjectPath.Contains(Text.ToString()); break;
		case EObjectSearchType::Class: bVisible = ClassName.Contains(Text.ToString()); break;
		case EObjectSearchType::Package: bVisible = ObjectPath.Contains(Text.ToString()); break;
		case EObjectSearchType::Module: bVisible = ClassPath.Contains(Text.ToString()); break;
		case EObjectSearchType::Function: bVisible = HasFunction(Text); break;
		default:
			break;
		}

		for (auto& Child : Children)
		{
			Child->OnSearch(Text, SearchType);
		}
	}

	bool FUObjectHolder::HasFunction(const FText& Text)
	{
		if (!ObjectPtr.IsValid()) return false;

		for (TFieldIterator<UFunction> FuncIt(ObjectPtr->GetClass(), EFieldIteratorFlags::IncludeSuper); FuncIt; ++FuncIt)
		{
			UFunction* Function = *FuncIt;
			if (Function->GetName().Contains(Text.ToString()))
				return true;
		}

		return false;
	}

	void FUObjectHolder::CloseSearch()
	{
		bVisible = true;
	}

	FText FObjectSearchType::GetText() const
	{
		switch (SearchType)
		{
		case EObjectSearchType::Name: return LOCTEXT("Name", "Name");
		case EObjectSearchType::Path: return LOCTEXT("Path", "Path");
		case EObjectSearchType::Class: return LOCTEXT("Class", "Class");
		case EObjectSearchType::Package: return LOCTEXT("Package", "Package");
		case EObjectSearchType::Module: return LOCTEXT("Module", "Module");
		case EObjectSearchType::Function: return LOCTEXT("Function", "Function");
		default: return LOCTEXT("NoneSearchType", "NoneSearchType");
		}
	}

}

#undef LOCTEXT_NAMESPACE