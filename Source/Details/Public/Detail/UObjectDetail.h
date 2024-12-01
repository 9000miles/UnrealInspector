// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DetailInfo.h"
#include "Core/DetailDefine.h"
#include "UObject/UnrealType.h"

namespace DETAILS_VIEWER
{
	namespace PROPERTY
	{
		class DETAILSVIEWER_API FUEPropertyHelper
		{
		public:
			static FString PropertyToJson(UObject* Object, FName Name);
			static void JsonToProperty(UObject* Object, FName Name, FString Json);


			static FString JsonValueToString(TSharedPtr<FJsonValue> JsonValue);
			static TSharedPtr<FJsonValue> StringToJsonValue(const FString& JsonString);

			static FString GetPropertyType(UE_Property* Property);
		};

		class DETAILSVIEWER_API FUEPropertyAccessor :public IPropertyAccessor
		{
		public:
			FUEPropertyAccessor(void* InContainer, UE_Property* InProperty, void* DefaultValue)
				:Property(InProperty),
				Container(InContainer),
				DefaultValuePtr(DefaultValue)
			{

			}
			virtual ~FUEPropertyAccessor() {}

		private:
			void Check(int32 Size)
			{
				checkf(Container && Property, TEXT("Container && Property is null"));
				checkf(Property->GetSize() == Size, TEXT("Property size is not equal to size"));
			}

			void Set(const void* In, int32 Size) override
			{
				Check(Size);

				Property->SetValue_InContainer(Container, In);
			}
			bool Get(void* Out, int32 Size) override
			{
				Check(Size);

				Property->GetValue_InContainer(Container, Out);
				return true;
			}

			//* ============================== Reset ================================= *//
			void Reset() {
				Check(Property->GetSize());

				Property->SetValue_InContainer(Container, DefaultValuePtr);
			}

			//* ============================== OnPropertyChanged ================================= *//
			virtual void OnPropertyChanged(FString MemberName, FString InnerName, EPropertyChangeAction Action)
			{

			}

		private:
			UE_Property* Property;
			void* Container;
			void* DefaultValuePtr;
			TWeakObjectPtr<UObject> OwnerObject;
		};

		class DETAILSVIEWER_API FUEConditionEvaluator :public IConditionEvaluator
		{
		public:
			FUEConditionEvaluator(TWeakObjectPtr<UObject> InObject, UE_Property* InProperty)
				:Object(InObject),
				Property(InProperty)
			{

			}
			virtual ~FUEConditionEvaluator() {}
			bool IsCanEditable() override;
			bool IsCanVisible() override;

		private:
			bool EvaluateSingleCondition(const FString& Condition, IConditionEvaluator::EType Type = EType::None) override;
			bool EvaluateCompareCondition(const FString& Condition, IConditionEvaluator::EType ComparisonType) const
			{
				TArray<FString> Parts;
				Condition.ParseIntoArray(Parts, *GetComparisonOperator(ComparisonType), true);
				if (Parts.Num() != 2) return false;

				const FString& PropertyName = Parts[0].TrimStartAndEnd();
				FString ConditionValue = Parts[1].TrimStartAndEnd();

				//UObject* Object = Property->GetOwnerUObject();
				UClass* Class = Object->GetClass();
				UE_Property* Ptr = Class->FindPropertyByName(*PropertyName);
				if (!Ptr) return false;

				const FString ValueStr = FUEPropertyHelper::PropertyToJson(Object.Get(), Ptr->GetFName());

				if (Ptr->IsA<FDoubleProperty>())
				{
					const double cValue = FCString::Atod(*ConditionValue);
					const double Value = FCString::Atod(*ValueStr);
					return CompareValues(Value, cValue, ComparisonType);
				}
				else if (Ptr->IsA<FIntProperty>())
				{
					const int32 cValue = FCString::Atoi(*ConditionValue);
					const int32 Value = FCString::Atoi(*ValueStr);
					return CompareValues(Value, cValue, ComparisonType);
				}
				else if (Ptr->IsA<FFloatProperty>())
				{
					const float cValue = FCString::Atof(*ConditionValue);
					const float Value = FCString::Atof(*ValueStr);
					return CompareValues(Value, cValue, ComparisonType);
				}
				else if (Ptr->IsA<FInt64Property>())
				{
					const int64 cValue = FCString::Atoi64(*ConditionValue);
					const int64 Value = FCString::Atoi64(*ValueStr);
					return CompareValues(Value, cValue, ComparisonType);
				}

				return false;
			}
			FString GetComparisonOperator(IConditionEvaluator::EType ComparisonType) const
			{
				switch (ComparisonType)
				{
				case IConditionEvaluator::EType::Less: return TEXT("<");
				case IConditionEvaluator::EType::Greater: return TEXT(">");
				case IConditionEvaluator::EType::LessEqual: return TEXT("<=");
				case IConditionEvaluator::EType::GreaterEqual: return TEXT(">=");
				default: return TEXT("");
				}
			}

			template<typename T>
			bool CompareValues(T Value, T ConditionValue, IConditionEvaluator::EType ComparisonType) const
			{
				switch (ComparisonType)
				{
				case IConditionEvaluator::EType::Less: return Value < ConditionValue;
				case IConditionEvaluator::EType::Greater: return Value > ConditionValue;
				case IConditionEvaluator::EType::LessEqual: return Value <= ConditionValue;
				case IConditionEvaluator::EType::GreaterEqual: return Value >= ConditionValue;
				default: return false;
				}
			}

		private:
			TWeakObjectPtr<UObject> Object;
			UE_Property* Property;
		};

		class DETAILSVIEWER_API FUEPropertyWidgetMaker :public IWidgetMaker
		{
		public:
			FUEPropertyWidgetMaker(UE_Property* InProperty)
				:Property(InProperty)
			{

			}
			virtual ~FUEPropertyWidgetMaker() {}

			TSharedRef<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) override;

		private:
			UE_Property* Property;
		};

		class DETAILSVIEWER_API FUEPropertyCopier :public ICopier
		{
		public:
			FUEPropertyCopier(TWeakObjectPtr<UObject> InObject, UE_Property* InProperty)
				:Property(InProperty),
				Object(InObject)
			{

			}
			virtual ~FUEPropertyCopier() {}
			const FString Execute() override;

		private:
			void* ContainerPtr;
			UE_Property* Property;
			TWeakObjectPtr<UObject> Object;
		};

		class DETAILSVIEWER_API FUEPropertyPaster :public IPaster
		{
		public:
			FUEPropertyPaster(TWeakObjectPtr<UObject> InObject, UE_Property* InProperty)
				:Property(InProperty),
				Object(InObject)
			{
			}
			virtual ~FUEPropertyPaster() {}
			void Execute(const FString String) override;

		private:
			void* ContainerPtr;
			UE_Property* Property;
			TWeakObjectPtr<UObject> Object;
		};

		class DETAILSVIEWER_API FUEPropertyMetadata :public FMetadata
		{
		public:
			FUEPropertyMetadata(UE_Property* InProperty)
				:Property(InProperty)
			{
				auto ssf = Property->GetMetaDataMap();
				Metadata = MakeShared<FJsonObject>();
				Metadata->SetStringField(TEXT("DisplayeName"), Property->GetMetaData(TEXT("DisplayeName")));
				Metadata->SetNumberField(TEXT("Min"), Property->GetFloatMetaData(TEXT("UIMin")));
				Metadata->SetNumberField(TEXT("Max"), Property->GetFloatMetaData(TEXT("UIMax")));
			}

		private:
			UE_Property* Property;
		};

		class DETAILSVIEWER_API FUObjectParameterExecutor :public IExecutor
		{
		public:
			FUObjectParameterExecutor(TWeakObjectPtr<UObject> InObject, UE_Property* InProperty, void* InContainer, void* DefaultValue)
				:Property(InProperty),
				Container(InContainer),
				DefaultValuePtr(DefaultValue),
				Object(InObject)
			{
				Accessor = MakeShareable(new PROPERTY::FUEPropertyAccessor(Container, Property, DefaultValuePtr));
				Condition = MakeShareable(new PROPERTY::FUEConditionEvaluator(Object, Property));
				WidgetMaker = MakeShareable(new PROPERTY::FUEPropertyWidgetMaker(Property));
				Copier = MakeShareable(new PROPERTY::FUEPropertyCopier(Object, Property));
				Paster = MakeShareable(new PROPERTY::FUEPropertyPaster(Object, Property));
			}
			virtual ~FUObjectParameterExecutor()
			{

			}

		protected:
			UE_Property* Property;
			void* Container;
			void* DefaultValuePtr;
			TWeakObjectPtr<UObject> Object;
		};


	}

	class FUObjectDetailCommander : public IDetailCommander
	{
	public:
		~FUObjectDetailCommander() {}
		static FString TypeName() { return TEXT("UObjectDetailCommander"); }
	};

	/**
	 * UObject Detail持有器
	 * 设置对象
	 * 获取Widget
	 */
	class DETAILSVIEWER_API FUObjectDetailHolder :public IDetailHolder
	{
	public:
		FUObjectDetailHolder();
		virtual ~FUObjectDetailHolder() {}

		void SetObject(TWeakObjectPtr<UObject> InObject);
		void SetObject();
		void IteratorField(TWeakObjectPtr<UObject> InObject, TSharedPtr<FCategoryList> CategoryList);

		TSharedPtr<FPropertyInfo> MakePropertyInfo(const FString PropertyName, const FString DisplayName, UE_Property* Property, void* DefaultValuePtr, FString Category, TWeakObjectPtr<UObject> InObject, void* Container);

		static FString TypeName() { return TEXT("UObjectDetail"); }
		FString GetTypeName() override { return FUObjectDetailHolder::TypeName(); }

		void SetDetailInfo(TSharedPtr<FDetailInfo> Info) override;

		TSharedPtr<SWidget> GetWidget() override;


		void Init(TSharedPtr<FDetailOptions> Options) override;

	private:
		TWeakObjectPtr<UObject> Object;
		TSharedPtr<FDetailInfo> DetailInfo;
		TSharedPtr<SDetailViewer> DetailViewer;
	};
}