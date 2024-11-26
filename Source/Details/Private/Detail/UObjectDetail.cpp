// Fill out your copyright notice in the Description page of Project Settings.
#include "Detail/UObjectDetail.h"
#include "JsonObjectConverter.h"
#include "View/SDetailViewer.h"

namespace DETAILS_VIEWER
{
	namespace PROPERTY
	{

#define LOCTEXT_NAMESPACE "DETAILS_VIEWER"

		bool FUEConditionEvaluator::IsCanEditable()
		{
			/**
			* 表达式类型大楷有如下这几种
				UPROPERTY(config, EditAnywhere, Category = "Animation Settings", meta = (EditCondition = "!bHideControlShapes"))
				UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bake", meta = (ClampMin = "1", UIMin = "1", EditCondition = "BakingKeySettings == EBakingKeySettings::AllFrames"))
				UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bake", meta = (EditCondition = "BakingKeySettings == EBakingKeySettings::AllFrames || bReduceKeys"))
				UPROPERTY(EditAnywhere, Category=ResourceDescription, meta=(EditCondition="ComponentBinding != nullptr", HideEditConditionToggle))
				UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Goal, meta=(NeverAsPin, EditCondition=false, EditConditionHides))
				UPROPERTY(EditAnywhere, Category = AnimationSharing, meta = (EditCondition = "bOnDemand", UIMin="0.0", UIMax="1.0", ClampMin = "0.0", ClampMax = "1.0"))
				UPROPERTY(EditAnywhere, meta = (DisplayName = "Target Space", Input, EditCondition = "Weight > 0.0" ), Category = "AimTarget")
				UPROPERTY(config, EditAnywhere, Category = "Enhanced Input|World Subsystem", meta = (editCondition = "bEnableDefaultMappingContexts && bEnableWorldSubsystem"))
				UPROPERTY(EditAnywhere, Category = Default, BlueprintReadWrite, meta = (EditCondition = "bEnabled && bUseCustomRange", EditConditionHides))
				UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Limits, meta = (EditCondition = "bLimitTranslation || bLimitRotation || bLimitScale"))
			**/
			const FString Condition = Property->GetMetaData("EditCondition");
			return Condition.IsEmpty() ? true : EvaluateCondition(Condition);
		}

		bool FUEConditionEvaluator::EvaluateSingleCondition(const FString& Condition, IConditionEvaluator::EType Type/* = EType::None*/)
		{
			switch (Type)
			{
			case DETAILS_VIEWER::PROPERTY::IConditionEvaluator::None:
				return true;
			case DETAILS_VIEWER::PROPERTY::IConditionEvaluator::Bool: {
				bool bNot = false;
				FString PropertyName = Condition;

				//UObject* Object = Property->GetOwnerUObject();
				UClass* Class = Object->GetClass();
				UE_Property* Ptr = Class->FindPropertyByName(*PropertyName);
				if (!Ptr) return false;

				const FString ValueStr = FUEPropertyHelper::PropertyToJson(Object.Get(), Ptr->GetFName());
				bool Result = ValueStr.Equals(TEXT("true"), ESearchCase::IgnoreCase);
				return bNot ? !Result : Result;
			}
			case DETAILS_VIEWER::PROPERTY::IConditionEvaluator::Equal: {
				TArray<FString> Conditions;
				Condition.ParseIntoArray(Conditions, TEXT("=="), true);
				if (Conditions.Num() != 2) return false;

				const FString& PropertyName = Conditions[0].TrimStartAndEnd();
				FString ConditionValue = Conditions[1].TrimStartAndEnd();

				//UObject* Object = Property->GetOwnerUObject();
				UClass* Class = Object->GetClass();
				UE_Property* Ptr = Class->FindPropertyByName(*PropertyName);
				if (!Ptr) return false;

				if (Ptr->IsA<FEnumProperty>()) {
					FEnumProperty* EnumProperty = CastFieldChecked<FEnumProperty>(Ptr);
					UEnum* Enum = EnumProperty->GetEnum();
					int64 EnumValue = Enum->GetValueByNameString(ConditionValue);
					uint8* EnumValuePtr = Ptr->ContainerPtrToValuePtr<uint8>(Object.Get());
					return EnumValuePtr && *EnumValuePtr == EnumValue;
				}

				const FString ValueStr = FUEPropertyHelper::PropertyToJson(Object.Get(), Ptr->GetFName());
				return ValueStr.Equals(ConditionValue, ESearchCase::IgnoreCase);
			}
			case DETAILS_VIEWER::PROPERTY::IConditionEvaluator::NotEqual: {
				TArray<FString> Conditions;
				Condition.ParseIntoArray(Conditions, TEXT("!="), true);
				if (Conditions.Num() != 2) return false;

				const FString& PropertyName = Conditions[0].TrimStartAndEnd();
				FString ConditionValue = Conditions[1].TrimStartAndEnd();

				//UObject* Object = Property->GetOwnerUObject();
				UClass* Class = Object->GetClass();
				UE_Property* Ptr = Class->FindPropertyByName(*PropertyName);
				if (!Ptr) return false;

				if (Ptr->IsA<FEnumProperty>()) {
					FEnumProperty* EnumProperty = CastFieldChecked<FEnumProperty>(Ptr);
					UEnum* Enum = EnumProperty->GetEnum();
					int64 EnumValue = Enum->GetValueByNameString(ConditionValue);
					uint8* EnumValuePtr = Ptr->ContainerPtrToValuePtr<uint8>(Object.Get());
					return EnumValuePtr && *EnumValuePtr != EnumValue;
				}

				const FString ValueStr = FUEPropertyHelper::PropertyToJson(Object.Get(), Ptr->GetFName());
				return !ValueStr.Equals(ConditionValue, ESearchCase::IgnoreCase);
			}
			case DETAILS_VIEWER::PROPERTY::IConditionEvaluator::Less:
			case DETAILS_VIEWER::PROPERTY::IConditionEvaluator::Greater:
			case DETAILS_VIEWER::PROPERTY::IConditionEvaluator::LessEqual:
			case DETAILS_VIEWER::PROPERTY::IConditionEvaluator::GreaterEqual:
				return EvaluateCompareCondition(Condition, Type);
			default:
				return true;
			}
		}

		bool FUEConditionEvaluator::IsCanVisible()
		{
			const FString Condition = Property->GetMetaData("VisibleCondition");
			return Condition.IsEmpty() ? true : EvaluateCondition(Condition);
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
			// @TODO: 不能使用Object
			return FUEPropertyHelper::PropertyToJson(Object.Get(), Property->GetFName());
		}

		void FUEPropertyPaster::Execute(const FString String)
		{
			// @TODO: 不能使用Object
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

		FString FUEPropertyHelper::GetPropertyType(UE_Property* Property)
		{
			if (Property->IsA<FEnumProperty>())	return "Enum";
			return Property->GetCPPType();
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
		}
		else
		{
			DetailInfo = MakeShareable(new FDetailInfo());
			DetailInfo->Name = Object->GetName();
			DetailInfo->DisplayName = Object->GetName();
			DetailInfo->Description = Object->GetName();
			DetailInfo->Commander = MakeShareable(new FUObjectDetailCommander());

			IteratorField(Object, DetailInfo->CategoryList);
		}

		SetDetailInfo(DetailInfo);
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
	}

	TSharedPtr<DETAILS_VIEWER::FPropertyInfo> FUObjectDetailHolder::MakePropertyInfo(const FString PropertyName, const FString DisplayName, UE_Property* Property, void* DefaultValuePtr, FString Category, TWeakObjectPtr<UObject> InObject, void* Container)
	{
		TSharedPtr<FPropertyInfo> PropertyInfo = MakeShareable(new FPropertyInfo());

		PropertyInfo->Name = PropertyName;
		PropertyInfo->DisplayName = DisplayName.IsEmpty() ? PropertyName : DisplayName;
		PropertyInfo->Description = Property->GetMetaData(TEXT("Description"));
		PropertyInfo->Type = PROPERTY::FUEPropertyHelper::GetPropertyType(Property);
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
		if (DetailInfo.IsValid())
		{
			DetailInfo->CategoryList->Sort();
		}

		if (DetailViewer.IsValid())
		{
			DetailViewer->SetDetailInfo(Info);
		}
	}

	TSharedPtr<SWidget> FUObjectDetailHolder::GetWidget()
	{
		//if (!Object.IsValid())
		//	return SNullWidget::NullWidget;

		if (DetailViewer.IsValid())
			return DetailViewer;

		DetailViewer = SNew(SDetailViewer)
			.DetailInfo(DetailInfo)
			;

		return DetailViewer;
	}

	void FUObjectDetailHolder::Init(TSharedPtr<FDetailOptions> Options)
	{
	}

}