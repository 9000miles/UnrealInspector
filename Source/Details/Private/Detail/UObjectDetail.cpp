// Fill out your copyright notice in the Description page of Project Settings.
#include "Detail/UObjectDetail.h"
#include "JsonObjectConverter.h"
#include "View/SDetailViewer.h"

namespace DETAILS_VIEWER
{
	namespace PROPERTY
	{

#define LOCTEXT_NAMESPACE "DETAILS_VIEWER"

		bool FUEPropertyEditable::CanEdit()
		{
			return true;
		}

		bool FUEPropertyVisible::CanVisible()
		{
			return true;
		}

		TSharedRef<SWidget> FUEPropertyWidgetMaker::MakeWidget(TSharedPtr<FTreeNode> Node)
		{
			TSharedPtr<FPropertyTreeNode> PropertyNode = StaticCastSharedPtr<FPropertyTreeNode>(Node);


			FString Type = PropertyNode->PropertyInfo->Type;
			TSharedPtr<IDetailWidgetCreater> WidgetCreater = FWidgetCreaterFactory::Get().FindCreater(Type);
			if (!WidgetCreater.IsValid())
			{
				const FText Message = FText::FromString(FString::Printf(TEXT("Unkown Type: %s"), *Type));
				return SNew(STextBlock)
					.Text(Message)
					;
			}

			return WidgetCreater->MakeWidget(Node).ToSharedRef();

			return SNew(SImage);
			return SNullWidget::NullWidget;
		}

		const FString FUEPropertyCopier::Execute()
		{
			return FUEPropertyHelper::PropertyToJson(Object.Get(), Property->GetFName());
		}

		void FUEPropertyPaster::Execute(const FString String)
		{
			FUEPropertyHelper::JsonToProperty(Object.Get(), Property->GetFName(), String);
		}

#define JSON_KEY FString(TEXT("Key"))

		FString FUEPropertyHelper::PropertyToJson(UObject* Object, FName Name)
		{
			if (Object == nullptr) return FString();

			FProperty* Property = Object->GetClass()->FindPropertyByName(Name);
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(Object);

			TSharedPtr<FJsonValue> JsonValue = FJsonObjectConverter::UPropertyToJsonValue(Property, ValuePtr);
			if (JsonValue.IsValid())
				return JsonValueToString(JsonValue);

			return FString();
		}

		void FUEPropertyHelper::JsonToProperty(UObject* Object, FName Name, FString Json)
		{
			if (Object == nullptr) return;

			FProperty* Property = Object->GetClass()->FindPropertyByName(Name);
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(Object);

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
#undef LOCTEXT_NAMESPACE

		//void FUEPropertySetter::SetValue(const bool value)
		//{
		//	if (!Object.IsValid() || Property == nullptr) return; FBoolProperty* Ptr = CastField<FBoolProperty>(Property); if (Ptr) {
		//		Ptr->SetPropertyValue_InContainer(Object.Get(), value);
		//	}
		//}

		//bool FUEPropertyGetter::GetValue()
		//{
		//	bool Result = false;
		//	Get(Result);
		//	return Result;
		//}

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
		UObject* CDO = Class->ClassDefaultObject.Get();
		for (TFieldIterator<UE_Property> PropertyIt(Class, EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
		{
			auto Property = *PropertyIt;
			const FString PropertyName = Property->GetName();
			const FString PropertyType = Property->GetCPPType();
			FString Category = Property->GetMetaData(TEXT("Category"));
			Category = Category.IsEmpty() ? TEXT("Default") : Category;
			const FString DisplayName = Property->GetMetaData(TEXT("DisplayName"));

			TSharedPtr<FCategoryInfo> ExistCategory = CategoryList->Find(Category);
			if (!ExistCategory.IsValid())
			{
				TSharedPtr<FCategoryInfo> CategoryInfo = MakeShareable(new FCategoryInfo());
				CategoryInfo->Name = Category;

				ExistCategory = CategoryInfo;
				CategoryList->Add(ExistCategory);
			}

			void* DefaultValuePtr = Property->ContainerPtrToValuePtr<void>(CDO);

			TSharedPtr<FPropertyInfo> PropertyInfo = MakePropertyInfo(
				PropertyName,
				DisplayName,
				Property,
				DefaultValuePtr,
				Category,
				InObject,
				InObject.Get()
			);

			ExistCategory->Add(PropertyInfo);
		}

		CategoryList->Sort();
	}

	FString FUObjectDetailHolder::GetPropertyType(UE_Property* Property)
	{
		if (Property->IsA<FEnumProperty>())	return "Enum";
		return Property->GetCPPType();
	}

	TSharedPtr<DETAILS_VIEWER::FPropertyInfo> FUObjectDetailHolder::MakePropertyInfo(const FString PropertyName, const FString DisplayName, UE_Property* Property, void* DefaultValuePtr, FString Category, TWeakObjectPtr<UObject> InObject, void* Container)
	{
		TSharedPtr<FPropertyInfo> PropertyInfo = MakeShareable(new FPropertyInfo());

		PropertyInfo->Name = PropertyName;
		PropertyInfo->DisplayName = DisplayName.IsEmpty() ? PropertyName : DisplayName;
		PropertyInfo->Description = Property->GetMetaData(TEXT("Description"));
		PropertyInfo->Type = GetPropertyType(Property);
		PropertyInfo->Category = Category;
		PropertyInfo->Advanced = Property->HasMetaData(TEXT("AdvancedDisplay"));
		PropertyInfo->Executor = MakeShareable(new PROPERTY::FUObjectParameterExecutor(InObject, Property, Container, DefaultValuePtr));
		PropertyInfo->Metadata = MakeShareable(new PROPERTY::FUEPropertyMetadata(Property));

		if (Property->IsA<FStructProperty>())
		{
			FStructProperty* StructProperty = CastFieldChecked<FStructProperty>(Property);
			TObjectPtr<class UScriptStruct> Struct = StructProperty->Struct;
			// Iterate over sub-properties of the struct
			for (TFieldIterator<UE_Property> It(Struct); It; ++It)
			{
				UE_Property* SubProperty = *It;
				void* SubDefaultValuePtr = SubProperty->ContainerPtrToValuePtr<void>(DefaultValuePtr);

				// Create sub-property info and add it to children list
				TSharedPtr<FPropertyInfo> SubPropertyInfo = MakePropertyInfo(
					SubProperty->GetName(),
					SubProperty->GetName(),
					SubProperty,
					SubDefaultValuePtr,
					TEXT(""),
					InObject,
					StructProperty->ContainerPtrToValuePtr<void>(Container)
				);
				PropertyInfo->Children.Add(SubPropertyInfo);
			}
		}
		else if (Property->IsA<FEnumProperty>())
		{
			FEnumProperty* EnumProperty = CastFieldChecked<FEnumProperty>(Property);
			UEnum* Enum = EnumProperty->GetEnum();

			TSharedPtr<FJsonObject> Metadata = PropertyInfo->Metadata->GetMetadata();
			TArray<TSharedPtr<FJsonValue>> EnumNames;
			// Add enum values and their display names to the property info
			for (int32 i = 0; i < Enum->NumEnums() - 1; ++i)
			{
				FString EnumValueDisplayName = Enum->GetNameStringByIndex(i);
				//FString EnumValueDisplayName = Enum->GetDisplayNameTextByIndex(i).ToString();

				EnumNames.Add(MakeShareable(new FJsonValueString(EnumValueDisplayName)));
			}
			Metadata->SetArrayField(TEXT("EnumNames"), EnumNames);
		}

		return PropertyInfo;
	}

	void FUObjectDetailHolder::SetDetailInfo(TSharedPtr<FDetailInfo> Info)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	TSharedPtr<SWidget> FUObjectDetailHolder::GetWidget()
	{
		if (!Object.IsValid())
			return SNullWidget::NullWidget;

		return SNew(SDetailViewer)
			.DetailInfo(DetailInfo)
			;
	}

	void FUObjectDetailHolder::Init(TSharedPtr<FDetailOptions> Options)
	{
	}

}