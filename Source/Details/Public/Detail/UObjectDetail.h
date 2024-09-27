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
			void Set(FString Value) {}
			void Set(int32 Value) {}

		private:
			UE_Property* Property;
		};

		class FUEPropertyGetter :public IGetter
		{

		};

		class FUEPropertyEditable :public IEditable
		{

		public:
			bool CanEdit() override;

		};

		class FUEPropertyVisible :public IVisible
		{

		public:
			bool CanVisible() override;

		};

		class FUEPropertyDefaultGetter :public IDefaultGetter
		{

		public:
			FString GetDefault() override;

		};

		class FUEPropertyWidgetMaker :public IWidgetMaker
		{

		public:
			TSharedRef<SWidget> MakeWidget(const FString& Value) override;

		};

		class FUEPropertyCopier :public ICopier
		{
		public:
			const FString Execute() override;

		private:
			void* ContainerPtr;
			UE_Property* Property;
		};

		class FUEPropertyPaster :public IPaster
		{
		public:
			void Execute(const FString String) override;

		private:
			void* ContainerPtr;
			UE_Property* Property;
		};

		class FUObjectParameterExecutor :public IExecutor
		{
		public:
			FUObjectParameterExecutor()
			{
				Setter = MakeShareable(new PROPERTY::FUEPropertySetter());
				Getter = MakeShareable(new PROPERTY::FUEPropertyGetter());
				Editable = MakeShareable(new PROPERTY::FUEPropertyEditable());
				Visible = MakeShareable(new PROPERTY::FUEPropertyVisible());
				DefaultGetter = MakeShareable(new PROPERTY::FUEPropertyDefaultGetter());
				WidgetMaker = MakeShareable(new PROPERTY::FUEPropertyWidgetMaker());
				CopyExecutor = MakeShareable(new PROPERTY::FUEPropertyCopier());
				PasteExecutor = MakeShareable(new PROPERTY::FUEPropertyPaster());
			}
			virtual ~FUObjectParameterExecutor()
			{

			}
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

	class FUObjectDetailExecutor : public IDetailExecutor
	{
	};

	/**
	 * $Comment$
	 */
	class FUObjectDetail :public ITypeName
	{
	public:
		FUObjectDetail();
		virtual ~FUObjectDetail() {}

		void SetObject(TWeakObjectPtr<UObject> InObject);
		void IteratorField(TWeakObjectPtr<UObject> InObject, TSharedPtr<FCategoryList> CategoryList);

		static FString TypeName() { return TEXT("UObjectDetail"); }
		FString GetTypeName() override { return FUObjectDetail::TypeName(); }

	private:
		TWeakObjectPtr<UObject> Object;
		TSharedPtr<FDetailInfo> DetailInfo;
	};
}