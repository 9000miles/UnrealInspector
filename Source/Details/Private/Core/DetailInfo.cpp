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
		DetailExecutor.Reset();
		DetailExecutor = nullptr;

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
		DetailExecutor = Factory::Get<IDetailExecutor>(ExecutorTypeName);
		DetailExecutor->FromJson(ExecutorJson);

		TArray<TSharedPtr<FJsonValue>> Array = JsonObject->GetArrayField(TEXT("CategoryList"));
		for (TSharedPtr<FJsonValue> Value : Array)
		{
			TSharedPtr<ICategoryInfo> CategoryInfo = MakeShared<ICategoryInfo>();
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
		JsonObject->SetObjectField(TEXT("DetailExecutor"), DetailExecutor->ToJson());
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

	void IDetailExecutor::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
		FString Maker;
		TSharedPtr<FJsonObject> DetailMakerJson = JsonObject->GetObjectField(IDetailExecutor::TypeName());
		if (DetailMakerJson->TryGetStringField(TYPE_NAME, Maker))
		{
			DetailMaker = Factory::Get<IDetailMaker>(Maker);
		}
		else
		{
			//UE_LOG(LogDetailsViewer, Error, TEXT("No DetailMaker"));
		}
	}

	TSharedPtr<FJsonObject> IDetailExecutor::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		JsonObject->SetStringField(TYPE_NAME, GetTypeName());
		JsonObject->SetObjectField(TEXT("DetailMaker"), DetailMaker->ToJson());

		return JsonObject;
	}

	FString IDetailExecutor::GetTypeName()
	{
		return IDetailExecutor::TypeName();
	}

	TMap<FString, TSharedPtr<ITypeName>> Factory::Map;

	ICategoryInfo::~ICategoryInfo()
	{
		CategoryExecutor.Reset();
		CategoryExecutor = nullptr;

		ParameterList.Reset();
		ParameterList = nullptr;
	}

	void ICategoryInfo::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
	}

	TSharedPtr<FJsonObject> ICategoryInfo::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("Name"), Name);
		JsonObject->SetStringField(TEXT("Description"), Description);
		JsonObject->SetStringField(TEXT("DisplayName"), DisplayName);
		JsonObject->SetObjectField(TEXT("CategoryExecutor"), CategoryExecutor->ToJson());
		JsonObject->SetObjectField(TEXT("ParameterList"), ParameterList->ToJson());

		return JsonObject;
	}

	void ICategoryInfo::Add(TSharedPtr<IParameterInfo> Parameter)
	{
		ParameterList->Add(Parameter);
	}

	void IParameterInfo::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
	}

	TSharedPtr<FJsonObject> IParameterInfo::ToJson()
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
		for (TSharedPtr<ICategoryInfo> Category : Categories)
			Array.Add(MakeShareable(new FJsonValueObject(Category->ToJson())));

		JsonObject->SetArrayField(TEXT("Categories"), Array);

		return JsonObject;
	}

	void FCategoryList::Add(TSharedPtr<ICategoryInfo> Category)
	{
		Categories.Add(Category);
	}



	TSharedPtr<ICategoryInfo> FCategoryList::Find(const FString& Name)
	{
		TSharedPtr<ICategoryInfo>* Found = Categories.FindByPredicate(
			[Name](TSharedPtr<ICategoryInfo> Item)
			{
				return Item->Name == Name;
			});

		return Found ? *Found : nullptr;
	}

	void FParameterList::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{

	}

	TSharedPtr<FJsonObject> FParameterList::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		TArray<TSharedPtr<FJsonValue>> Array;
		for (TSharedPtr<IParameterInfo> Parameter : Parameters)
			Array.Add(MakeShareable(new FJsonValueObject(Parameter->ToJson())));

		JsonObject->SetArrayField(TEXT("Categories"), Array);

		return JsonObject;
	}

	void FParameterList::Add(TSharedPtr<IParameterInfo> Parameter)
	{

	}

	TSharedPtr<IParameterInfo> FParameterList::Find(const FString& Name)
	{
		TSharedPtr<IParameterInfo>* Found = Parameters.FindByPredicate(
			[Name](TSharedPtr<IParameterInfo> Item)
			{
				return Item->Name == Name;
			});

		return Found ? *Found : nullptr;
	}
}