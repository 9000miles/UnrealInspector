// Fill out your copyright notice in the Description page of Project Settings.
#include "Core/DetailInfo.h"

namespace DETAILS_VIEWER
{
#define TYPE_NAME TEXT("TYPE_NAME")

	TSharedPtr<SWidget> FCustomDetailMaker::MakeWidget()
	{
		return SNullWidget::NullWidget;
	}

	FString FCustomDetailMaker::GetTypeName()
	{
		return  TEXT("CustomDetailMaker");
	}

	FDetailInfo::FDetailInfo()
	{
		CategoryList = MakeShared<FCategoryList>();
	}

	FDetailInfo::~FDetailInfo()
	{
		Commander.Reset();
		Commander = nullptr;

		CategoryList.Reset();
		CategoryList = nullptr;
	}

	void FDetailInfo::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
		Name = JsonObject->GetStringField(TEXT("Name"));
		Description = JsonObject->GetStringField(TEXT("Description"));
		DisplayName = JsonObject->GetStringField(TEXT("DisplayName"));

		TSharedPtr<FJsonObject> ExecutorJson = JsonObject->GetObjectField(TEXT("DetailExecutor"));
		FString ExecutorTypeName = ExecutorJson->GetStringField(TYPE_NAME);
		Commander = Factory::Get<IDetailCommander>(ExecutorTypeName);
		Commander->FromJson(ExecutorJson);

		TArray<TSharedPtr<FJsonValue>> Array = JsonObject->GetArrayField(TEXT("CategoryList"));
		for (TSharedPtr<FJsonValue> Value : Array)
		{
			TSharedPtr<FCategoryInfo> CategoryInfo = MakeShared<FCategoryInfo>();
			CategoryInfo->FromJson(Value->AsObject());
			CategoryList->Add(CategoryInfo);
		}

	}

	TSharedPtr<FJsonObject> FDetailInfo::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("Name"), Name);
		JsonObject->SetStringField(TEXT("Description"), Description);
		JsonObject->SetStringField(TEXT("DisplayName"), DisplayName);
		JsonObject->SetObjectField(TEXT("DetailExecutor"), Commander->ToJson());
		JsonObject->SetObjectField(TEXT("CategoryList"), CategoryList->ToJson());

		return JsonObject;
	}

	void FDetailInfo::Merge(TSharedPtr<FDetailInfo> Other)
	{

	}

	void FDetailInfo::Merge(TArray<TSharedPtr<FDetailInfo>> Others)
	{

	}

	TSharedPtr<SWidget> FDetailMaker::MakeWidget()
	{
		return SNullWidget::NullWidget;
	}

	FString FDetailMaker::GetTypeName()
	{
		return  TEXT("DetailMaker");
	}

	void IDetailCommander::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
		FString Maker;
		TSharedPtr<FJsonObject> DetailMakerJson = JsonObject->GetObjectField(IDetailCommander::TypeName());
		if (DetailMakerJson->TryGetStringField(TYPE_NAME, Maker))
		{
			DetailMaker = Factory::Get<IDetailMaker>(Maker);
		}
		else
		{
			//UE_LOG(LogDetailsViewer, Error, TEXT("No DetailMaker"));
		}
	}

	TSharedPtr<FJsonObject> IDetailCommander::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		JsonObject->SetStringField(TYPE_NAME, GetTypeName());
		JsonObject->SetObjectField(TEXT("DetailMaker"), DetailMaker->ToJson());

		return JsonObject;
	}

	FString IDetailCommander::GetTypeName()
	{
		return IDetailCommander::TypeName();
	}

	TMap<FString, TSharedPtr<ITypeName>> Factory::Map;

	FCategoryInfo::~FCategoryInfo()
	{
		CategoryExecutor.Reset();
		CategoryExecutor = nullptr;

		PropertyList.Reset();
		PropertyList = nullptr;
	}

	void FCategoryInfo::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
	}

	TSharedPtr<FJsonObject> FCategoryInfo::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("Name"), Name);
		JsonObject->SetStringField(TEXT("Description"), Description);
		JsonObject->SetStringField(TEXT("DisplayName"), DisplayName);
		JsonObject->SetObjectField(TEXT("CategoryExecutor"), CategoryExecutor->ToJson());
		JsonObject->SetObjectField(TEXT("ParameterList"), PropertyList->ToJson());

		return JsonObject;
	}

	void FCategoryInfo::Add(TSharedPtr<FPropertyInfo> Parameter)
	{
		PropertyList->Add(Parameter);
	}

	void FCategoryInfo::Sort()
	{
		PropertyList->Sort();
	}

	void FPropertyInfo::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
	}

	TSharedPtr<FJsonObject> FPropertyInfo::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

		JsonObject->SetStringField(TEXT("Name"), Name);
		JsonObject->SetStringField(TEXT("Description"), Description);
		JsonObject->SetStringField(TEXT("DisplayName"), DisplayName);
		JsonObject->SetStringField(TEXT("Type"), Type);
		JsonObject->SetStringField(TEXT("Category"), Category);
		JsonObject->SetBoolField(TEXT("Advanced"), Advanced);
		JsonObject->SetObjectField(TEXT("Executor"), Executor->ToJson());
		JsonObject->SetObjectField(TEXT("Metadata"), Metadata->ToJson());

		return JsonObject;
	}

	void PROPERTY::IExecutor::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
	}

	TSharedPtr<FJsonObject> PROPERTY::IExecutor::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		JsonObject->SetObjectField(TEXT("Setter"), Setter->ToJson());
		JsonObject->SetObjectField(TEXT("Getter"), Getter->ToJson());
		JsonObject->SetObjectField(TEXT("Editable"), Editable->ToJson());
		JsonObject->SetObjectField(TEXT("Visible"), Visible->ToJson());
		JsonObject->SetObjectField(TEXT("DefaultGetter"), DefaultGetter->ToJson());
		JsonObject->SetObjectField(TEXT("WidgetMaker"), WidgetMaker->ToJson());

		return JsonObject;
	}

	void PROPERTY::FMetadata::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
		Metadata = JsonObject;
	}

	TSharedPtr<FJsonObject> PROPERTY::FMetadata::ToJson()
	{
		return Metadata;
	}

	void FCategoryList::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{

	}

	TSharedPtr<FJsonObject> FCategoryList::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		TArray<TSharedPtr<FJsonValue>> Array;
		for (TSharedPtr<FCategoryInfo> Category : Categories)
			Array.Add(MakeShareable(new FJsonValueObject(Category->ToJson())));

		JsonObject->SetArrayField(TEXT("Categories"), Array);

		return JsonObject;
	}

	void FCategoryList::Add(TSharedPtr<FCategoryInfo> Category)
	{
		Categories.Add(Category);
	}



	TSharedPtr<FCategoryInfo> FCategoryList::Find(const FString& Name)
	{
		TSharedPtr<FCategoryInfo>* Found = Categories.FindByPredicate(
			[Name](TSharedPtr<FCategoryInfo> Item)
			{
				return Item->Name == Name;
			});

		return Found ? *Found : nullptr;
	}

	void FCategoryList::Sort()
	{
		Categories.Sort([](TSharedPtr<FCategoryInfo> ItemA, TSharedPtr<FCategoryInfo> ItemB)
			{
				return ItemA->Name < ItemB->Name;
			});

		for (TSharedPtr<FCategoryInfo> Category : Categories)
		{
			Category->Sort();
		}
	}

	void FCategoryList::Enumerate(TFunction<void(TSharedPtr<FCategoryInfo>)> Func)
	{
		for (auto Item : Categories)
		{
			Func(Item);
		}
	}

	void FPropertyList::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{

	}

	TSharedPtr<FJsonObject> FPropertyList::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		TArray<TSharedPtr<FJsonValue>> Array;
		for (TSharedPtr<FPropertyInfo> Parameter : Parameters)
			Array.Add(MakeShareable(new FJsonValueObject(Parameter->ToJson())));

		JsonObject->SetArrayField(TEXT("Categories"), Array);

		return JsonObject;
	}

	void FPropertyList::Add(TSharedPtr<FPropertyInfo> Parameter)
	{

	}

	TSharedPtr<FPropertyInfo> FPropertyList::Find(const FString& Name)
	{
		TSharedPtr<FPropertyInfo>* Found = Parameters.FindByPredicate(
			[Name](TSharedPtr<FPropertyInfo> Item)
			{
				return Item->Name == Name;
			});

		return Found ? *Found : nullptr;
	}

	void FPropertyList::Sort()
	{
		Parameters.Sort([](TSharedPtr<FPropertyInfo> ItemA, TSharedPtr<FPropertyInfo> ItemB)
			{
				return ItemA->Name < ItemB->Name;
			});
	}

	void FPropertyList::Enumerate(TFunction<void(TSharedPtr<FPropertyInfo>)> Func)
	{
		for (auto Item : Parameters)
		{
			Func(Item);
		}
	}

}