// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DetailInfo.h"
#include "JsonObjectConverter.h"

using namespace DETAILS_VIEWER;

namespace UOBJECT_COLLECTOR { class FFunctionHolder; }

class FFunctionParameterAccessor :public PROPERTY::IPropertyAccessor
{
public:
	FFunctionParameterAccessor(TWeakObjectPtr<UFunction> InFunction, UE_Property* InProperty, uint8* InContainer)
		:Property(InProperty),
		Container(InContainer),
		Function(InFunction)
	{
		bIsReturnValue = Property->HasAnyPropertyFlags(CPF_ReturnParm | CPF_OutParm);
	}

	bool Get(void* Out, int32 Size) override
	{
		check(Container && Property);
		check(Size == Property->GetSize());

		int32 Offset = Property->GetOffset_ForInternal();
		if (!bDefaultValueCached && !bIsReturnValue)
		{
			TMap<FName, FString>* MetaMap = UMetaData::GetMapForObject(Function.Get());
			const FName MetadataCppDefaultValueKey(*(FString(TEXT("CPP_Default_")) + Property->GetName()));
			FString* DefaultValueStringPtr = MetaMap->Find(MetadataCppDefaultValueKey);
			if (!DefaultValueStringPtr) return false;

			FString DefaultValue = *DefaultValueStringPtr;
			TSharedPtr<FJsonValue> JsonValue = PROPERTY::FUEPropertyHelper::StringToJsonValue(DefaultValue);
			FText OutReason;
			if (FJsonObjectConverter::JsonValueToUProperty(
				JsonValue,
				Property,
				Container + Offset,
				0,
				0,
				false,
				&OutReason))
			{
				bDefaultValueCached = true;
			}
		}

		Property->GetValue_InContainer(Container, Out);
		return true;
	}

	void Set(const void* In, int32 Size) override
	{
		check(Container && Property);
		check(Size == Property->GetSize());

		Property->SetValue_InContainer(Container, In);

		bDefaultValueCached = true;
	}

	virtual void Reset() {}

	virtual void OnPropertyChanged(FString MemberName, FString InnerName, PROPERTY::EPropertyChangeAction Action) {}

private:
	UE_Property* Property;
	uint8* Container;
	void* DefaultValuePtr;
	bool bDefaultValueCached = false;
	bool bIsReturnValue;
	TWeakObjectPtr<UFunction> Function;
};

class FFunctionParameterConditionEvaluator :public PROPERTY::IConditionEvaluator
{
public:
	FFunctionParameterConditionEvaluator(TWeakObjectPtr < UFunction> InFunction, UE_Property* InProperty)
		:Function(InFunction),
		Property(InProperty)
	{
	}
	virtual ~FFunctionParameterConditionEvaluator() {}

public:
	virtual bool IsCanEditable() { return true; }
	virtual bool IsCanVisible() { return true; }
	virtual bool EvaluateSingleCondition(const FString& Condition, EType Type) { return true; }

private:
	TWeakObjectPtr < UFunction> Function;
	UE_Property* Property;
};
// 定义一个接口
class IMyInterface
{
public:
	virtual ~IMyInterface() = default;

	// 纯虚函数
	virtual void doSomething() = 0;

	// 模板函数，不是虚函数
	template<typename T>
	void process(T value)
	{
		// 默认行为
	}
};

// 实现这个接口的类
class MyImplementation : public IMyInterface
{
public:
	// 实现纯虚函数
	void doSomething() override
	{
		// 具体实现
	}


	// 重载模板函数
	void process(int value)
	{
		// 针对 int 类型的具体实现
	}

	void process(double value)
	{
		// 针对 double 类型的具体实现
	}
};
class FFunctionParameterExecutor :public PROPERTY::IExecutor
{
public:
	FFunctionParameterExecutor(TWeakObjectPtr<UFunction> InFunction, UE_Property* InProperty, uint8* InContainer)
		:Property(InProperty),
		Function(InFunction)
	{
		Accessor = MakeShareable(new FFunctionParameterAccessor(Function, InProperty, InContainer));

		Condition = MakeShareable(new PROPERTY::FUEConditionEvaluator(Function, Property));
		WidgetMaker = MakeShareable(new PROPERTY::FUEPropertyWidgetMaker(Property));
		Copier = MakeShareable(new PROPERTY::FUEPropertyCopier(Function, Property));
		Paster = MakeShareable(new PROPERTY::FUEPropertyPaster(Function, Property));
	}

protected:
	UE_Property* Property;
	TWeakObjectPtr<UFunction> Function;
};

class FFunctionDetailHolder :public IDetailHolder
{
	IMPLEMENT_ITYPENAME(FFunctionDetailHolder)
public:
	virtual ~FFunctionDetailHolder();
	void Init(TSharedPtr<FDetailOptions> Options) override;

	TSharedPtr<SWidget> GetWidget() override;

	void SetFunction(TSharedPtr<FFunctionHolder> FunctionHolder);
	void InvokeFunction();

protected:
	void SetDetailInfo(TSharedPtr<FDetailInfo> Info) override;

private:
	TSharedPtr<FFunctionHolder> Holder;
	TSharedPtr<FDetailInfo> DetailInfo;
	TSharedPtr<SDetailViewer> DetailViewer;

	uint8* ParameterPtr = nullptr;
};
/**
 * Comment
 */
class SFunctionViewer : public SCompoundWidget
{
	enum EFuncitonAccess
	{
		Static,
		Public,
		Protected,
		Private,
	};
public:
	SLATE_BEGIN_ARGS(SFunctionViewer)
		{
		}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void SetObject(UObject* InObject);
	TSharedRef<SWidget> MakeFunctionGroup(EFuncitonAccess Access);
	TSharedRef<SWidget> MakeFunctionList(EFuncitonAccess Access);

	TSharedRef<class ITableRow> GenerateRowWidget(TSharedPtr<FFunctionHolder> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnSelectionChanged(TSharedPtr<FFunctionHolder> NewSelection, ESelectInfo::Type SelectInfo);
	FReply OnExecute();

private:
	TWeakObjectPtr<UObject> Object;
	TSharedPtr<SVerticalBox> FunctionGroups;

	TArray<TSharedPtr<FFunctionHolder>> StaticFunctions;
	TArray<TSharedPtr<FFunctionHolder>> PublicFunctions;
	TArray<TSharedPtr<FFunctionHolder>> ProtectedFunctions;
	TArray<TSharedPtr<FFunctionHolder>> PrivateFunctions;

	TArray<TSharedPtr<FFunctionHolder>> FunctionList;

	TSharedPtr< FFunctionDetailHolder> FunctionDetailHolder;
	TStrongObjectPtr<UAAA> AAA;
};