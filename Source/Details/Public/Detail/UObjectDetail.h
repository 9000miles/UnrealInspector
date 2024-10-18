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
		class FUEPropertySetter :public ISetter
		{
		public:
			FUEPropertySetter(UE_Property* InProperty)
				:Property(InProperty)
			{

			}
			virtual ~FUEPropertySetter() {}
			void Set(FString Value) {}
			void Set(int32 Value) {}

		private:
			UE_Property* Property;
		};

		class FUEPropertyGetter :public IGetter
		{
		public:
			FUEPropertyGetter(UE_Property* InProperty)
				:Property(InProperty)
			{

			}
			virtual ~FUEPropertyGetter() {}

			template<typename T>
			T Get()
			{
				return *(T*)Property->ContainerPtrToValuePtr<void>(Object.Get());
			}

		private:
			UE_Property* Property;
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

		class FUEPropertyDefaultGetter :public IDefaultGetter
		{
		public:
			FUEPropertyDefaultGetter(TWeakObjectPtr<UObject> InObject, UE_Property* InProperty)
				:Property(InProperty),
				Object(InObject)
			{
			}
			virtual ~FUEPropertyDefaultGetter() {}

			FString GetDefault() override;

			UE_Property* Property;
			TWeakObjectPtr<UObject> Object;
		};

		class FUEPropertyWidgetMaker :public IWidgetMaker
		{
		public:
			FUEPropertyWidgetMaker(UE_Property* InProperty)
				:Property(InProperty)
			{

			}
			virtual ~FUEPropertyWidgetMaker() {}

			TSharedRef<SWidget> MakeWidget() override;

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
			virtual ~FUEPropertyCopier(){}
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
				Setter = MakeShareable(new PROPERTY::FUEPropertySetter(Property));
				Getter = MakeShareable(new PROPERTY::FUEPropertyGetter(Property));
				Editable = MakeShareable(new PROPERTY::FUEPropertyEditable(Property));
				Visible = MakeShareable(new PROPERTY::FUEPropertyVisible(Property));
				DefaultGetter = MakeShareable(new PROPERTY::FUEPropertyDefaultGetter(Object, Property));
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
			static FString PropertyToJson(void* ContainerPtr, UE_Property* Property);
			static void JsonToProperty(void* ContainerPtr, UE_Property* Property, FString Json);

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
	 * $Comment$
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