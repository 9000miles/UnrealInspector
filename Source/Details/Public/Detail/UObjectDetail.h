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
			FUEPropertyAccessor(TWeakObjectPtr<UObject> InObject, UE_Property* InProperty)
				:Property(InProperty),
				Object(InObject)
			{

			}
			virtual ~FUEPropertyAccessor() {}

			//* ============================== Set ================================= *//
			void Set(FString value) { SetValue<FString, FStrProperty>(value); }
			void Set(FName value) { SetValue<FName, FNameProperty>(value); }
			void Set(FText value) { SetValue<FText, FTextProperty>(value); }
			//void Set(FVector value) { SetValue<FVector, FStructProperty>(value); }
			void Set(uint16 value) { SetValue<uint16, FUInt16Property>(value); }
			void Set(uint32 value) { SetValue<uint32, FUInt32Property>(value); }
			void Set(uint64 value) { SetValue<uint64, FUInt64Property>(value); }
			void Set(int8 value) { SetValue<int8, FInt8Property>(value); }
			void Set(int16 value) { SetValue<int16, FInt16Property>(value); }
			void Set(int32 value) { SetValue<int32, FIntProperty>(value); }
			void Set(int64 value) { SetValue<int64, FInt64Property>(value); }
			void Set(bool value) { SetValue<bool, FBoolProperty>(value); }
			void Set(uint8 value) { SetValue<uint8, FByteProperty>(value); }
			void Set(float value) { SetValue<float, FFloatProperty>(value); }
			void Set(double value) { SetValue<double, FDoubleProperty>(value); }
			void Set(UObject* value) { SetValue<UObject*, FObjectProperty>(value); }
			void Set(UClass* value) { SetValue<UClass*, FClassProperty>(value); }

#define DEFINE_SET_FUNC_CONTAINER(Type, PropType) \
			template<typename T> \
			void Set(const Type<T>& value) { \
				if (!Object.IsValid() || Property == nullptr) return; \
				PropType* Ptr = CastField<PropType>(Property); \
				if (Ptr) Ptr->SetPropertyValue_InContainer(Object.Get(), value); \
			}
			DEFINE_SET_FUNC_CONTAINER(TArray, FArrayProperty);
			DEFINE_SET_FUNC_CONTAINER(TSet, FSetProperty);

			// 宏定义 Map 类型
#define DEFINE_SET_FUNC_MAP(PropType) \
			template<typename K, typename V> \
			void Set(const TMap<K, V>& value) {\
				if (!Object.IsValid() || Property == nullptr) return; \
				PropType* Ptr = CastField<PropType>(Property); \
				if (Ptr) Ptr->SetPropertyValue_InContainer(Object.Get(), value); \
			}
			DEFINE_SET_FUNC_MAP(FMapProperty);

			//* ============================== Get ================================= *//
			void Get(bool& Out) { GetValue<bool, FBoolProperty>(Out); }
			void Get(float& Out) { GetValue<float, FFloatProperty>(Out); }
			void Get(double& Out) { GetValue<double, FDoubleProperty>(Out); }
			void Get(int32& Out) { GetValue<int32, FIntProperty>(Out); }
			void Get(FString& Out) { GetValue<FString, FStrProperty>(Out); }
			void Get(FName& Out) { GetValue<FName, FNameProperty>(Out); }
			void Get(FText& Out) { GetValue<FText, FTextProperty>(Out); }

			//* ============================== Reset ================================= *//
			void Reset() {
				if (!Object.IsValid()) return;

				UObject* CDO = Object->GetClass()->ClassDefaultObject.Get();
				if (CDO == nullptr || Property == nullptr) return;

				FString DefaultValue = FUEPropertyHelper::PropertyToJson(CDO, Property->GetFName());
				FUEPropertyHelper::JsonToProperty(Object.Get(), Property->GetFName(), DefaultValue);
			}

			//* ============================== OnPropertyChanged ================================= *//
			virtual void OnPropertyChanged(FString MemberName, FString InnerName, EPropertyChangeAction Action)
			{

			}

		protected:
			template<typename TValue, typename TPropertyType>
			void SetValue(TValue value) {
				if (!Object.IsValid() || Property == nullptr) return;

				TPropertyType* Ptr = CastField<TPropertyType>(Property);
				if (Ptr) Ptr->SetPropertyValue_InContainer(Object.Get(), value);

				OnPropertyChanged(Property->GetName(), TEXT(""), EPropertyChangeAction::Unspecified);
			}

			template<typename TValue, typename TPropertyType>
			void GetValue(TValue& Out) {
				if (!Object.IsValid() || Property == nullptr) return;

				TPropertyType* Ptr = CastField<TPropertyType>(Property);
				if (Ptr) Out = Ptr->GetPropertyValue_InContainer(Object.Get());
			}

		private:
			UE_Property* Property;
			TWeakObjectPtr<UObject> Object;
		};

		class FUEPropertyEditable :public IEditable
		{
		public:
			FUEPropertyEditable(UE_Property* InProperty)
				:Property(InProperty)
			{

			}
			virtual ~FUEPropertyEditable() {}
			bool CanEdit() override;

		private:
			UE_Property* Property;
		};

		class FUEPropertyVisible :public IVisible
		{
		public:
			FUEPropertyVisible(UE_Property* InProperty)
				:Property(InProperty)
			{

			}
			virtual ~FUEPropertyVisible() {}
			bool CanVisible() override;

		private:
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
				Metadata = MakeShared<FJsonObject>();
			}

		private:
			UE_Property* Property;
		};

		class FUObjectParameterExecutor :public IExecutor
		{
		public:
			FUObjectParameterExecutor(TWeakObjectPtr<UObject> InObject, UE_Property* InProperty)
				:Property(InProperty),
				Object(InObject)
			{
				Accessor = MakeShareable(new PROPERTY::FUEPropertyAccessor(Object, Property));
				Editable = MakeShareable(new PROPERTY::FUEPropertyEditable(Property));
				Visible = MakeShareable(new PROPERTY::FUEPropertyVisible(Property));
				WidgetMaker = MakeShareable(new PROPERTY::FUEPropertyWidgetMaker(Property));
				CopyExecutor = MakeShareable(new PROPERTY::FUEPropertyCopier(Object, Property));
				PasteExecutor = MakeShareable(new PROPERTY::FUEPropertyPaster(Object, Property));
			}
			virtual ~FUObjectParameterExecutor()
			{

			}

		protected:
			UE_Property* Property;
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