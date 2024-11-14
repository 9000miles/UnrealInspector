// Fill out your copyright notice in the Description page of Project Settings.
#include "Core/DetailInfo.h"
#include "Node/DetailNode.h"

namespace DETAILS_VIEWER
{
#define TYPE_NAME TEXT("TYPE_NAME")

	TSharedPtr<SWidget> FCustomDetailMaker::MakeWidget(TSharedPtr<FTreeNode> Node)
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

	TSharedPtr<SWidget> FDetailMaker::MakeWidget(TSharedPtr<FTreeNode> Node)
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

	FCategoryInfo::FCategoryInfo()
	{
		//CategoryExecutor = MakeShared<ICategoryExecutor>();
		PropertyList = MakeShared<FPropertyList>();
	}

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

	FString FCategoryInfo::Copy()
	{
		const FString JsonString = PropertyList->Copy();
		return FString::Printf(TEXT("{%s}"), *JsonString);
	}

	void FCategoryInfo::Paste(const FString& String)
	{
		PropertyList->Paste(String);
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

	FString FPropertyInfo::Copy()
	{
		return Executor->Copier->Execute();
	}

	void FPropertyInfo::Paste(const FString& String)
	{
		Executor->Paster->Execute(String);
	}

	void FPropertyInfo::Enumerate(TFunction<void(TSharedPtr<FPropertyInfo>)> Func)
	{
		// �ȶԵ�ǰ�ڵ�ִ�� Func
		Func(AsShared());

		// �������ӽڵ�ݹ�ִ�� Func
		for (TSharedPtr<FPropertyInfo> Item : Children)
		{
			Item->Enumerate(Func);
		}
	}

	void FPropertyInfo::MakeTreeNode(TSharedPtr<FTreeNode> ParentNode)
	{
		TSharedPtr<FPropertyTreeNode> TreeNode = MakeShareable(new FPropertyTreeNode(AsShared()));
		ParentNode->AddChild(TreeNode);
		TreeNode->SetParent(ParentNode);

		TSharedPtr<IDetailWidgetCreater> WidgetCreater = FWidgetCreaterFactory::Get().FindCreater(Type);
		if (WidgetCreater.IsValid() && !WidgetCreater->IsAllowHasChildren())
			return;

		for (TSharedPtr<FPropertyInfo> Child : Children)
		{
			Child->MakeTreeNode(TreeNode);
		}
	}

	void PROPERTY::IExecutor::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
	}

	TSharedPtr<FJsonObject> PROPERTY::IExecutor::ToJson()
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

		JsonObject->SetObjectField(TEXT("Setter"), Accessor->ToJson());
		JsonObject->SetObjectField(TEXT("Editable"), Condition->ToJson());
		JsonObject->SetObjectField(TEXT("WidgetMaker"), WidgetMaker->ToJson());

		return JsonObject;
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
		Parameters.Add(Parameter);
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

	FString FPropertyList::Copy()
	{
		FString Result;
		for (TSharedPtr<FPropertyInfo> Item : Parameters)
		{
			Result += FString::Printf(TEXT("\"%s\":%s,"), *Item->Name, *Item->Copy());
		}
		// Remove last comma
		Result.RemoveFromEnd(",");
		return Result;
	}

	void FPropertyList::Paste(const FString& String)
	{
		// ���� JSON �ַ���
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);

		if (!FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON string: %s"), *String);
			return;
		}

		for (TSharedPtr<FPropertyInfo> Item : Parameters)
		{
			FString PropertyName = Item->Name;
			if (!JsonObject->HasField(PropertyName)) continue;

			// ��ȡ JSON �����е�ֵ
			TSharedPtr<FJsonValue> JsonValue = JsonObject->TryGetField(PropertyName);

			if (!JsonValue.IsValid()) continue;

			FString Value = ConvertJsonValueToString(JsonValue);

			// ���� FPropertyInfo �� Paste ����
			Item->Paste(Value);
		}
	}

	FString FPropertyList::ConvertJsonValueToString(TSharedPtr<FJsonValue> JsonValue)
	{
		FString Value;

		switch (JsonValue->Type)
		{
		case EJson::Boolean:
			Value = JsonValue->AsBool() ? TEXT("true") : TEXT("false");
			break;
		case EJson::Number:
			Value = FString::SanitizeFloat(JsonValue->AsNumber());
			break;
		case EJson::String:
			Value = JsonValue->AsString();
			break;
		case EJson::Object:
		{
			// ����Ƕ�׵Ľṹ��
			TSharedPtr<FJsonObject> NestedObject = JsonValue->AsObject();
			if (NestedObject.IsValid())
			{
				// ��Ƕ�׵� JSON ����ת��Ϊ�ַ���
				FString NestedJsonString;
				TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&NestedJsonString, /*Indent=*/0);
				FJsonSerializer::Serialize(NestedObject.ToSharedRef(), JsonWriter);
				Value = NestedJsonString;
			}
			break;
		}
		case EJson::Array:
		{
			// ��������
			TArray<TSharedPtr<FJsonValue>> ArrayValues = JsonValue->AsArray();
			FString ArrayString;
			for (const TSharedPtr<FJsonValue>& ArrayValue : ArrayValues)
			{
				if (ArrayString.Len() > 0)
				{
					ArrayString += TEXT(",");
				}
				ArrayString += ConvertJsonValueToString(ArrayValue);
			}
			Value = TEXT("[") + ArrayString + TEXT("]");
			break;
		}
		default:
			break;
		}

		return Value;
	}

	void PROPERTY::IConditionEvaluator::FromJson(TSharedPtr<FJsonObject> JsonObject)
	{
	}

	TSharedPtr<FJsonObject> PROPERTY::IConditionEvaluator::ToJson()
	{
		return MakeShared<FJsonObject>();
	}

}
