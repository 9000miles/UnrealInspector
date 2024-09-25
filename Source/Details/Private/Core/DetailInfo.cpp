// Fill out your copyright notice in the Description page of Project Settings.
#include "Core/DetailInfo.h"

namespace DetailsViewer
{


	TSharedPtr<SWidget> FCustomDetailMaker::MakeWidget()
	{
		return SNullWidget::NullWidget;
	}

	FString FCustomDetailMaker::GetTypeName()
	{
		return  TEXT("CustomDetailMaker");
	}

	void IDetailInfo::FromJSON(TSharedPtr<FJsonObject> JsonObject)
	{
		Name = JsonObject->GetStringField(TEXT("Name"));
		Description = JsonObject->GetStringField(TEXT("Description"));
		DisplayName = JsonObject->GetStringField(TEXT("DisplayName"));
		DetailExecutor->FromJSON(JsonObject->GetObjectField(TEXT("DetailExecutor")));

		TArray<TSharedPtr<FJsonValue>> Array = JsonObject->GetArrayField(TEXT("CategoryList"));
		for (TSharedPtr<FJsonValue> Value : Array)
		{
			TSharedPtr<ICategoryInfo> CategoryInfo = MakeShared<ICategoryInfo>();
			CategoryInfo->FromJSON(Value->AsObject());
			CategoryList.Add(CategoryInfo);
		}

	}

	TSharedPtr<FJsonObject> IDetailInfo::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("Name"), Name);
		JsonObject->SetStringField(TEXT("Description"), Description);
		JsonObject->SetStringField(TEXT("DisplayName"), DisplayName);
		JsonObject->SetObjectField(TEXT("DetailExecutor"), DetailExecutor->ToJson());

		TArray<TSharedPtr<FJsonValue>> Array;
		for (TSharedPtr<ICategoryInfo> Category : CategoryList)
			Array.Add(MakeShareable(new FJsonValueObject(Category->ToJson())));
		JsonObject->SetArrayField(TEXT("CategoryList"), Array);

		return JsonObject;
	}

	TSharedPtr<SWidget> FDetailMaker::MakeWidget()
	{
		return SNullWidget::NullWidget;
	}

	FString FDetailMaker::GetTypeName()
	{
		return  TEXT("DetailMaker");
	}

	void IDetailExecutor::FromJSON(TSharedPtr<FJsonObject> JsonObject)
	{
		FString Maker;
		if (JsonObject->TryGetStringField(TEXT("DetailMaker"), Maker))
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
		return MakeShared<FJsonObject>();
	}

	TMap<FString, TSharedPtr<ITypeName>> Factory::Map;
}