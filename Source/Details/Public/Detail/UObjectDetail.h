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
#define DEFINE_SET_FUNC(Type, PropType) \
			void Set(Type value) { \
				if (!Object.IsValid() || Property == nullptr) return; \
				PropType* Ptr = CastField<PropType>(Property); \
				if (Ptr) Ptr->SetPropertyValue_InContainer(Object.Get(), value); \
			}
			DEFINE_SET_FUNC(FString, FStrProperty);
			DEFINE_SET_FUNC(FName, FNameProperty);
			DEFINE_SET_FUNC(FText, FTextProperty);


			//DEFINE_SET_FUNC(uint8, FUInt8Property);
			DEFINE_SET_FUNC(uint16, FUInt16Property);
			DEFINE_SET_FUNC(uint32, FUInt32Property);
			DEFINE_SET_FUNC(uint64, FUInt64Property);

			DEFINE_SET_FUNC(int8, FInt8Property);
			DEFINE_SET_FUNC(int16, FInt16Property);
			DEFINE_SET_FUNC(int32, FIntProperty);
			DEFINE_SET_FUNC(int64, FInt64Property);

			DEFINE_SET_FUNC(bool, FBoolProperty);
			DEFINE_SET_FUNC(uint8, FByteProperty);
			DEFINE_SET_FUNC(float, FFloatProperty);
			DEFINE_SET_FUNC(double, FDoubleProperty);


			DEFINE_SET_FUNC(UObject*, FObjectProperty);
			DEFINE_SET_FUNC(UClass*, FClassProperty);

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
#define DEFINE_GET_FUNC(Type, PropType) \
			void Get(Type& Out) { \
				if (!Object.IsValid() || Property == nullptr) return; \
				PropType* Ptr = CastField<PropType>(Property); \
				if (Ptr) Out = Ptr->GetPropertyValue_InContainer(Object.Get()); \
			}

			DEFINE_GET_FUNC(bool, FBoolProperty);
			DEFINE_GET_FUNC(float, FFloatProperty);
			DEFINE_GET_FUNC(double, FDoubleProperty);
			DEFINE_GET_FUNC(int32, FIntProperty);
			DEFINE_GET_FUNC(FString, FStrProperty);
			DEFINE_GET_FUNC(FName, FNameProperty);
			DEFINE_GET_FUNC(FText, FTextProperty);

			//* ============================== Default ================================= *//
#define DEFINE_DEFAULT_FUNC(Type, PropType)\
			void Default(Type& Out)\
			{\
				if (!Object.IsValid() || Property == nullptr) return;\
				PropType* Ptr = CastField<PropType>(Property); \
				if (Ptr) Out = Ptr->GetPropertyValue_InContainer(Object.Get()); \
			}

			DEFINE_DEFAULT_FUNC(bool, FBoolProperty);
			DEFINE_DEFAULT_FUNC(float, FFloatProperty);
			DEFINE_DEFAULT_FUNC(double, FDoubleProperty);
			DEFINE_DEFAULT_FUNC(int32, FIntProperty);
			DEFINE_DEFAULT_FUNC(FString, FStrProperty);
			DEFINE_DEFAULT_FUNC(FName, FNameProperty);
			DEFINE_DEFAULT_FUNC(FText, FTextProperty);

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


		class FUEPropertyHelper
		{
		public:
			static FString PropertyToJson(UObject* Object, FName Name);
			static void JsonToProperty(UObject* Object, FName Name, FString Json);


			static FString JsonValueToString(TSharedPtr<FJsonValue> JsonValue);
			static TSharedPtr<FJsonValue> StringToJsonValue(const FString& JsonString);

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