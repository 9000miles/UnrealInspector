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
		class FUEPropertyHelper
		{
		public:
			static FString PropertyToJson(UObject* Object, FName Name);
			static void JsonToProperty(UObject* Object, FName Name, FString Json);


			static FString JsonValueToString(TSharedPtr<FJsonValue> JsonValue);
			static TSharedPtr<FJsonValue> StringToJsonValue(const FString& JsonString);

		};

		class FUEPropertyAccessor :public IPropertyAccessor
		{
		public:
			FUEPropertyAccessor(void* InContainer, UE_Property* InProperty, void* DefaultValue)
				:Property(InProperty),
				Container(InContainer),
				DefaultValuePtr(DefaultValue)
			{

			}
			virtual ~FUEPropertyAccessor() {}

			//* ============================== Set ================================= *//
			void Set(FString Value) { SetValue<FString, FStrProperty>(Value); }
			void Set(FName Value) { SetValue<FName, FNameProperty>(Value); }
			void Set(FText Value) { SetValue<FText, FTextProperty>(Value); }
			void Set(FGuid Value) { SetStructValue<FGuid>(Value); }
			void Set(FVector2D Value) { SetStructValue<FVector2D>(Value); }
			void Set(FVector Value) { SetStructValue<FVector>(Value); }
			void Set(FVector4 Value) { SetStructValue<FVector4>(Value); }
			void Set(FTransform Value) { SetStructValue<FTransform>(Value); }
			void Set(uint16 Value) { SetValue<uint16, FUInt16Property>(Value); }
			void Set(uint32 Value) { SetValue<uint32, FUInt32Property>(Value); }
			void Set(uint64 Value) { SetValue<uint64, FUInt64Property>(Value); }
			void Set(int8 Value) { SetValue<int8, FInt8Property>(Value); }
			void Set(int16 Value) { SetValue<int16, FInt16Property>(Value); }
			void Set(int32 Value) { SetValue<int32, FIntProperty>(Value); }
			void Set(int64 Value) { SetValue<int64, FInt64Property>(Value); }
			void Set(bool Value) { SetValue<bool, FBoolProperty>(Value); }
			void Set(uint8 Value) { SetValue<uint8, FByteProperty>(Value); }
			void Set(FEnumValue Value) {
				if (!Container || Property == nullptr) return;

				FEnumProperty* Ptr = CastField<FEnumProperty>(Property);
				check(Ptr);

				UEnum* Enum = Ptr->GetEnum();
				const uint8 NewEnumValue = Value.Value;
				if (NewEnumValue < 0 || NewEnumValue >= Enum->NumEnums())
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid enum value: %d"), NewEnumValue);
					return;
				}
				uint8* EnumValuePtr = Ptr->ContainerPtrToValuePtr<uint8>(Container);
				*EnumValuePtr = NewEnumValue;
			}
			void Set(float Value) { SetValue<float, FFloatProperty>(Value); }
			void Set(double Value) { SetValue<double, FDoubleProperty>(Value); }
			void Set(UObject* Value) { SetValue<UObject*, FObjectProperty>(Value); }
			void Set(UClass* Value) { SetValue<UClass*, FClassProperty>(Value); }

#define DEFINE_SET_FUNC_CONTAINER(Type, PropType) \
			template<typename T> \
			void Set(const Type<T>& Value) { \
				if (!Container || Property == nullptr) return; \
				PropType* Ptr = CastField<PropType>(Property); \
				if (Ptr) Ptr->SetPropertyValue_InContainer(Container, Value); \
			}
			DEFINE_SET_FUNC_CONTAINER(TArray, FArrayProperty);
			DEFINE_SET_FUNC_CONTAINER(TSet, FSetProperty);

			// 宏定义 Map 类型
#define DEFINE_SET_FUNC_MAP(PropType) \
			template<typename K, typename V> \
			void Set(const TMap<K, V>& Value) {\
				if (!Container || Property == nullptr) return; \
				PropType* Ptr = CastField<PropType>(Property); \
				if (Ptr) Ptr->SetPropertyValue_InContainer(Container, Value); \
			}
			DEFINE_SET_FUNC_MAP(FMapProperty);


			//* ============================== Get ================================= *//
			void Get(bool& Out) { GetValue<bool, FBoolProperty>(Out); }
			void Get(float& Out) { GetValue<float, FFloatProperty>(Out); }
			void Get(double& Out) { GetValue<double, FDoubleProperty>(Out); }
			void Get(uint8& Out) { GetValue<uint8, FByteProperty>(Out); }
			void Get(FEnumValue& Out) {
				if (!Container || Property == nullptr) return;

				FEnumProperty* Ptr = CastField<FEnumProperty>(Property);
				check(Ptr);

				uint8* EnumValuePtr = Ptr->ContainerPtrToValuePtr<uint8>(Container);
				Out.Value = *EnumValuePtr;
			}
			void Get(int32& Out) { GetValue<int32, FIntProperty>(Out); }
			void Get(FString& Out) { GetValue<FString, FStrProperty>(Out); }
			void Get(FName& Out) { GetValue<FName, FNameProperty>(Out); }
			void Get(FText& Out) { GetValue<FText, FTextProperty>(Out); }
			void Get(FGuid& Out) { GetStructValue<FGuid>(Out); }
			void Get(FVector2D& Out) { GetStructValue<FVector2D>(Out); }
			void Get(FVector& Out) { GetStructValue<FVector>(Out); }
			void Get(FVector4& Out) { GetStructValue<FVector4>(Out); }
			void Get(FTransform& Out) { GetStructValue<FTransform>(Out); }

			//* ============================== Reset ================================= *//
			void Reset() {
				if (!Container) return;

				void* ValuePtr = Property->ContainerPtrToValuePtr<void>(Container);
				if (!ValuePtr || !DefaultValuePtr) return; // 检查指针是否为空

				// 使用 memcpy 进行内存拷贝，避免直接解引用 void* 指针
				memcpy(ValuePtr, DefaultValuePtr, Property->GetSize());
			}

			//* ============================== OnPropertyChanged ================================= *//
			virtual void OnPropertyChanged(FString MemberName, FString InnerName, EPropertyChangeAction Action)
			{

			}

		protected:
			template<typename TValue, typename TPropertyType>
			void SetValue(TValue Value) {
				if (!Container || Property == nullptr) return;

				TPropertyType* Ptr = CastField<TPropertyType>(Property);
				check(Ptr);
				if (Ptr) Ptr->SetPropertyValue_InContainer(Container, Value);

				OnPropertyChanged(Property->GetName(), TEXT(""), EPropertyChangeAction::Unspecified);
			}
			template<typename TValue>
			void SetStructValue(TValue Value)
			{
				if (!Container || Property == nullptr) return;

				FStructProperty* StructProperty = CastField<FStructProperty>(Property);
				UScriptStruct* StructType = StructProperty->Struct;
				check(StructType == TBaseStructure<TValue>::Get());

				StructProperty->SetValue_InContainer(Container, &Value);

				OnPropertyChanged(Property->GetName(), TEXT(""), EPropertyChangeAction::Unspecified);
			}


			template<typename TValue, typename TPropertyType>
			void GetValue(TValue& Out) {
				if (!Container || Property == nullptr) return;

				TPropertyType* Ptr = CastField<TPropertyType>(Property);
				check(Ptr);
				if (Ptr) Out = Ptr->GetPropertyValue_InContainer(Container);
			}

			template<typename TValue>
			void GetStructValue(TValue& Out)
			{
				if (!Container || Property == nullptr) return;

				FStructProperty* StructProperty = CastField<FStructProperty>(Property);
				UScriptStruct* StructType = StructProperty->Struct;
				check(StructType == TBaseStructure<TValue>::Get());

				StructProperty->GetValue_InContainer(Container, &Out);
			}

		private:
			UE_Property* Property;
			void* Container;
			void* DefaultValuePtr;
			TWeakObjectPtr<UObject> OwnerObject;
		};

		class FUEConditionEvaluator :public IConditionEvaluator
		{
		public:
			FUEConditionEvaluator(TWeakObjectPtr<UObject> InObject, UE_Property* InProperty)
				:Object(InObject),
				Property(InProperty)
			{

			}
			virtual ~FUEConditionEvaluator() {}
			bool CanEdit() override;
			bool CanVisible() override;

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

		class FUEPropertyWidgetMaker :public IWidgetMaker
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

		class FUEPropertyCopier :public ICopier
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

		class FUEPropertyPaster :public IPaster
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

		class FUEPropertyMetadata :public FMetadata
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

		class FUObjectParameterExecutor :public IExecutor
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
	class FUObjectDetailHolder :public IDetailHolder
	{
	public:
		FUObjectDetailHolder();
		virtual ~FUObjectDetailHolder() {}

		void SetObject(TWeakObjectPtr<UObject> InObject);
		void SetObject();
		void IteratorField(TWeakObjectPtr<UObject> InObject, TSharedPtr<FCategoryList> CategoryList);

		FString GetPropertyType(UE_Property* Property);
		TSharedPtr<FPropertyInfo> MakePropertyInfo(const FString PropertyName, const FString DisplayName, UE_Property* Property, void* DefaultValuePtr, FString Category, TWeakObjectPtr<UObject> InObject, void* Container);

		static FString TypeName() { return TEXT("UObjectDetail"); }
		FString GetTypeName() override { return FUObjectDetailHolder::TypeName(); }

		void SetDetailInfo(TSharedPtr<FDetailInfo> Info) override;

		TSharedPtr<SWidget> GetWidget() override;


		void Init(TSharedPtr<FDetailOptions> Options) override;

	private:
		TWeakObjectPtr<UObject> Object;
		TSharedPtr<FDetailInfo> DetailInfo;
	};
}