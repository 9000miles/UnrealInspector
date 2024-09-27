// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DetailInfo.h"
#include "Core/DetailDefine.h"
#include "UObject/UnrealType.h"

namespace DETAILS_VIEWER
{
	namespace PARAMETER
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

		class FUEPropertyCopyExecutor :public ICopyExecutor
		{

		public:
			void Execute() override;

		};

		class FUEPropertyPasteExecutor :public IPasteExecutor
		{

		public:
			void Execute() override;

		};

		class FUObjectParameterExecutor :public IExecutor
		{
		public:
			FUObjectParameterExecutor()
			{
				Setter = MakeShareable(new PARAMETER::FUEPropertySetter());
				Getter = MakeShareable(new PARAMETER::FUEPropertyGetter());
				Editable = MakeShareable(new PARAMETER::FUEPropertyEditable());
				Visible = MakeShareable(new PARAMETER::FUEPropertyVisible());
				DefaultGetter = MakeShareable(new PARAMETER::FUEPropertyDefaultGetter());
				WidgetMaker = MakeShareable(new PARAMETER::FUEPropertyWidgetMaker());
				CopyExecutor = MakeShareable(new PARAMETER::FUEPropertyCopyExecutor());
				PasteExecutor = MakeShareable(new PARAMETER::FUEPropertyPasteExecutor());
			}
			virtual ~FUObjectParameterExecutor()
			{

			}
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