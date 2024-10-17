// Fill out your copyright notice in the Description page of Project Settings.
#include "Detail/UObjectDetail.h"
#include "JsonObjectConverter.h"
#include "View/SDetailView.h"

namespace DETAILS_VIEWER
{
	namespace PROPERTY
	{

		bool FUEPropertyEditable::CanEdit()
		{
			return true;
		}

		bool FUEPropertyVisible::CanVisible()
		{
			return true;
		}

		FString FUEPropertyDefaultGetter::GetDefault()
		{
			return TEXT("");
		}

		TSharedRef<SWidget> FUEPropertyWidgetMaker::MakeWidget()
		{
			return SNullWidget::NullWidget;
		}

		const FString FUEPropertyCopier::Execute()
		{
			return FUEPropertyHelper::PropertyToJson(ContainerPtr, Property);
		}

		void FUEPropertyPaster::Execute(const FString String)
		{
			FUEPropertyHelper::JsonToProperty(ContainerPtr, Property, String);
		}

#define JSON_KEY FString(TEXT("Key"))

		FString FUEPropertyHelper::PropertyToJson(void* ContainerPtr, UE_Property* Property)
		{
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(ContainerPtr);

			TSharedPtr<FJsonValue> JsonValue = FJsonObjectConverter::UPropertyToJsonValue(Property, ValuePtr);
			if (JsonValue.IsValid())
				return JsonValueToString(JsonValue);

			return FString();
		}

		void FUEPropertyHelper::JsonToProperty(void* ContainerPtr, UE_Property* Property, FString Json)
		{
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(ContainerPtr);

			TSharedPtr<FJsonValue> JsonValue = StringToJsonValue(Json);
			FText OutReason;
			if (!FJsonObjectConverter::JsonValueToUProperty(JsonValue, Property, ValuePtr, 0, 0, false, &OutReason))
			{
				UE_LOG(LogTemp, Error, TEXT("JsonToProperty: %s"), *OutReason.ToString());
			}
		}

		FString FUEPropertyHelper::JsonValueToString(TSharedPtr<FJsonValue> JsonValue)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
			JsonObject->SetField(JSON_KEY, JsonValue);

			FString JsonString;
			TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonString, /*Indent=*/0);

			if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter))
			{
				const int SN = 4;
				const int EN = JSON_KEY.Len() + SN + 1;
				return JsonString.Mid(JSON_KEY.Len() + SN, JsonString.Len() - EN);
			}

			return JsonString;
		}

		TSharedPtr<FJsonValue> FUEPropertyHelper::StringToJsonValue(const FString& String)
		{
			FString JsonString = FString::Printf(TEXT("{\"%s\":%s}"), *JSON_KEY, *String);
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
			TSharedPtr<FJsonValue> JsonValue;
			if (FJsonSerializer::Deserialize(JsonReader, JsonValue))
			{
				return JsonValue->AsObject()->TryGetField(JSON_KEY);
				return JsonValue;
			}

			return MakeShareable(new FJsonValueNull());
		}
	}

	FUObjectDetailHolder::FUObjectDetailHolder()
	{

	}

	void FUObjectDetailHolder::SetObject(TWeakObjectPtr<UObject> InObject)
	{
		Object = InObject;

		if (!Object.IsValid())
		{
			DetailInfo.Reset();
			DetailInfo = nullptr;
			return;
		}

		DetailInfo = MakeShareable(new FDetailInfo());
		DetailInfo->Name = Object->GetName();
		DetailInfo->DisplayName = Object->GetName();
		DetailInfo->Description = Object->GetName();
		DetailInfo->Commander = MakeShareable(new FUObjectDetailCommander());

		IteratorField(Object, DetailInfo->CategoryList);
	}

	void FUObjectDetailHolder::SetObject()
	{
		SetObject(nullptr);
	}

	void FUObjectDetailHolder::IteratorField(TWeakObjectPtr<UObject> InObject, TSharedPtr<FCategoryList> CategoryList)
	{
		check(InObject.IsValid());

		UClass* Class = InObject->GetClass();
		for (TFieldIterator<UE_Property> PropertyIt(Class, EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
		{
			auto Property = *PropertyIt;
			const FString PropertyName = Property->GetName();
			const FString PropertyType = Property->GetCPPType();
			const FString Category = Property->GetMetaData(TEXT("Category"));
			const FString DisplayName = Property->GetMetaData(TEXT("DisplayName"));

			TSharedPtr<FCategoryInfo> ExistCategory = CategoryList->Find(Category);
			if (!ExistCategory.IsValid())
			{
				TSharedPtr<FCategoryInfo> CategoryInfo = MakeShareable(new FCategoryInfo());
				ExistCategory = CategoryInfo;
				CategoryList->Add(CategoryInfo);
			}

			TSharedPtr<FPropertyInfo> Parameter = MakeShareable(new FPropertyInfo());

			Parameter->Name = PropertyName;
			Parameter->DisplayName = DisplayName;
			Parameter->Description = Property->GetMetaData(TEXT("Description"));
			Parameter->Type = Property->GetCPPType();
			Parameter->Category = Category;
			Parameter->Advanced = Property->HasMetaData(TEXT("AdvancedDisplay"));
			Parameter->Executor = MakeShareable(new PROPERTY::FUObjectParameterExecutor());
			Parameter->Metadata = MakeShareable(new PROPERTY::FMetadata());

			ExistCategory->Add(Parameter);
		}

		CategoryList->Sort();
	}

	void FUObjectDetailHolder::SetDetailInfo(TSharedPtr<FDetailInfo> Info)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	TSharedPtr<SWidget> FUObjectDetailHolder::GetWidget()
	{
		return SNew(SDetailView)
			.DetailInfo(DetailInfo)
			;
	}

	void FUObjectDetailHolder::Init(TSharedPtr<FDetailOptions> Options)
	{
	}

}