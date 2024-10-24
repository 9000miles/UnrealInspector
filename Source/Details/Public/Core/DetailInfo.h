// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Interface/ITypeName.h"
#include "Interface/IJsonable.h"

namespace DETAILS_VIEWER { class FTreeNode; }

namespace DETAILS_VIEWER
{
	class IExecutorData
	{
	};

	class IExecutor
	{
	public:
		virtual void Execute() = 0;
	};

	class Factory
	{
	public:
		template<typename T> requires std::derived_from<T, ITypeName>
		static TSharedPtr<T> Get(const FString& TypeName)
		{
			if (!Map.Contains(TypeName)) return nullptr;

			const TSharedPtr<ITypeName> Instance = Map[TypeName];
			return StaticCastSharedPtr<T>(Instance);
		}
		template<typename T> requires std::derived_from<T, ITypeName>
		static bool Register()
		{
			TSharedPtr<T> Instance = MakeShared<T>();
			Instance->Initalized();
			FString TypeName = Instance->GetTypeName();
			if (Map.Contains(TypeName)) return false;

			Map.Add(TypeName, Instance);
			return true;
		}
		static bool Unregister(const FString& TypeName)
		{
			if (!Map.Contains(TypeName)) return false;

			return Map.Remove(TypeName) > 0;
		}
	private:
		static TMap<FString, TSharedPtr<ITypeName>> Map;
	};

	/**
	 * ϸ����幹����
	 */
	class IDetailMaker :public IJsonable, public ITypeName
	{
	public:
		virtual ~IDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) = 0;
	};

	class FDetailMaker : public IDetailMaker
	{
	public:
		virtual ~FDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) override;

		FString GetTypeName() override;

	};

	class FCustomDetailMaker :public IDetailMaker
	{
	public:
		virtual ~FCustomDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) override;

		FString GetTypeName() override;

	};

	class ICopier
	{
	public:
		virtual ~ICopier() {}
		virtual const FString Execute() = 0;
	};

	class IPaster
	{
	public:
		virtual ~IPaster() {}
		virtual void Execute(const FString String) = 0;
	};

	/**
	 * 细节面板指挥官
	 * 负责创建细节面板，整个面板的复制和粘贴
	 */
	class IDetailCommander :public IJsonable, public ITypeName
	{
	public:
		virtual ~IDetailCommander() {}
		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		static FString TypeName() { return TEXT("DetailCommander"); }
		FString GetTypeName() override;

	public:
		TSharedPtr<IDetailMaker> DetailMaker;
		TSharedPtr<ICopier> CopyExecutor;
		TSharedPtr<IPaster> PasteExecutor;
	};

	class ICategoryExecutor :public IJsonable
	{
	public:
		virtual void Execute() = 0;
	};

	namespace PROPERTY
	{
		class IPropertyAccessor :public IJsonable
		{
		public:
			virtual void Set(const bool Value) = 0;
			virtual void Set(const float Value) = 0;
			virtual void Set(const double Value) = 0;
			//virtual void Set(const int Value) = 0;
			//virtual void Set(const uint8 Value) = 0;
			//virtual void Set(const int8 Value) = 0;
			virtual void Set(const int32 Value) = 0;
			virtual void Set(const FString Value) = 0;
			virtual void Set(const FName Value) = 0;
			virtual void Set(const FText Value) = 0;
			//virtual void Set(const FVector Value) = 0;

			virtual void Get(bool& Out) = 0;
			virtual void Get(float& Out) = 0;
			virtual void Get(double& Out) = 0;
			//virtual void Get(int& Out) = 0;
			//virtual void Get(uint8& Out) = 0;
			//virtual void Get(int8& Out) = 0;
			virtual void Get(int32& Out) = 0;
			virtual void Get(FString& Out) = 0;
			virtual void Get(FName& Out) = 0;
			virtual void Get(FText& Out) = 0;
			//virtual void Get(FVector& Out) = 0;

			virtual void Reset() = 0;
		};

		class IEditable :public IJsonable
		{
		public:
			virtual bool CanEdit() = 0;
		};
		class IVisible :public IJsonable
		{
		public:
			virtual bool CanVisible() = 0;
		};
		class IWidgetMaker :public IJsonable
		{
		public:
			virtual TSharedRef<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) = 0;
		};

		class IExecutor :public IJsonable
		{
		public:
			IExecutor()
			{
			}
			virtual ~IExecutor()
			{
			}
			void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
			TSharedPtr<FJsonObject> ToJson() override;

		public:
			TSharedPtr<IPropertyAccessor> Accessor;
			TSharedPtr<IEditable> Editable;
			TSharedPtr<IVisible> Visible;
			TSharedPtr<IWidgetMaker> WidgetMaker;
			TSharedPtr<ICopier> CopyExecutor;
			TSharedPtr<IPaster> PasteExecutor;
		};

		class FMetadata :public IJsonable
		{
		public:
			virtual ~FMetadata()
			{
				Metadata.Reset();
				Metadata = nullptr;
			}
			void FromJson(TSharedPtr<FJsonObject> JsonObject) override { Metadata = JsonObject; }
			TSharedPtr<FJsonObject> ToJson() override { return Metadata; }

		private:
			template<typename T>
			T GetValue(const FString& Key);

			template<>
			bool GetValue<bool>(const FString& Key) {
				bool Result;
				Metadata->TryGetBoolField(Key, Result);
				return Result;
			}

			template<>
			FString GetValue<FString>(const FString& Key) {
				FString Result;
				Metadata->TryGetStringField(Key, Result);
				return Result;
			}

			template<typename T>
			T GetNumberValue(const FString& Key) {
				T Result;
				Metadata->TryGetNumberField(Key, Result);
				return Result;
			}

			template<typename T>
			TArray<T> GetArray(const FString& Key) {
				TArray<T> ResultArray;
				const TArray<TSharedPtr<FJsonValue>>* JsonArray;

				if (Metadata->TryGetArrayField(Key, JsonArray)) {
					for (const TSharedPtr<FJsonValue>& Value : *JsonArray) {
						T Element;
						if constexpr (std::is_same_v<T, FString>)
							Element = Value->AsString();
						else if constexpr (std::is_same_v<T, bool>)
							Element = Value->AsBool();
						else
							Value->TryGetNumber(Element);
						ResultArray.Add(Element);
					}
				}

				return ResultArray;
			}

		public:
			template<typename T>
			T Get(const FString& Key) { return GetNumberValue<T>(Key); }

			template<>
			FString Get(const FString& Key) { return GetValue<FString>(Key); }

			template<>
			FText Get(const FString& Key) { return FText::FromString(GetValue<FString>(Key)); }

			template<>
			bool Get(const FString& Key) { return GetValue<bool>(Key); }

			template<>
			TArray<FString> Get(const FString& Key) { return GetArray<FString>(Key); }

			template<>
			TSharedPtr<FJsonObject> Get(const FString& Key) { return Metadata->GetObjectField(Key); }

			template<>
			TArray<TSharedPtr<FJsonValue>> Get(const FString& Key) {
				TArray<TSharedPtr<FJsonValue>> Result;
				const TArray<TSharedPtr<FJsonValue>>* JsonArray;

				if (!Metadata->TryGetArrayField(Key, JsonArray))
					return Result;

				for (const TSharedPtr<FJsonValue>& Value : *JsonArray)
					Result.Add(Value);

				return Result;
			}

			TSharedPtr<FJsonObject> GetMetadata() { return Metadata; }

		protected:
			TSharedPtr<FJsonObject> Metadata;
		};

		class FArrayMetadata
		{
		public:
			FString ElementType;
		};

		class FMapMetadata
		{
		public:
			FString KeyType;
			FString ValueType;
		};

		class FEnumMetadata
		{
		public:
			TArray<FString> EnumValues;
		};
	}





	class FPropertyInfo :public IJsonable
	{
	public:
		virtual ~FPropertyInfo()
		{
		}
		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

	public:
		FString Name;
		FString Description;
		FString DisplayName;
		FString Type;
		FString Category;
		bool Advanced;
		TSharedPtr<PROPERTY::IExecutor> Executor;
		TSharedPtr<PROPERTY::FMetadata> Metadata;

	};

	class FPropertyList :public IJsonable
	{
	public:
		virtual ~FPropertyList()
		{
		}
		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		void Add(TSharedPtr<FPropertyInfo> Parameter);
		TSharedPtr<FPropertyInfo> Find(const FString& Name);

		virtual void Sort();
		void Enumerate(TFunction<void(TSharedPtr<FPropertyInfo>)> Func);

	private:
		TArray<TSharedPtr<FPropertyInfo>> Parameters;
	};

	class FCategoryInfo :public IJsonable
	{
	public:
		FCategoryInfo();
		virtual ~FCategoryInfo();

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		void Add(TSharedPtr<FPropertyInfo> Parameter);
		virtual void Sort();

	public:
		FString Name;
		FString Description;
		FString DisplayName;
		TSharedPtr<ICategoryExecutor> CategoryExecutor;
		TSharedPtr<FPropertyList> PropertyList;
	};

	class FCategoryList :public IJsonable
	{
	public:
		virtual ~FCategoryList()
		{
		}
		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		void Add(TSharedPtr<FCategoryInfo> Category);
		TSharedPtr<FCategoryInfo> Find(const FString& Name);

		virtual void Sort();
		void Enumerate(TFunction<void(TSharedPtr<FCategoryInfo>)> Func);

	private:
		TArray<TSharedPtr<FCategoryInfo>> Categories;
	};

	/**
	 * $Comment$
	 */
	class FDetailInfo :public IJsonable
	{
	public:
		FDetailInfo();
		virtual ~FDetailInfo();

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		void Merge(TArray<TSharedPtr<FDetailInfo>> Others);
		void Merge(TSharedPtr<FDetailInfo> Other);

	public:
		FString Name;
		FString Description;
		FString DisplayName;
		TSharedPtr<IDetailCommander> Commander;
		TSharedPtr<FCategoryList> CategoryList;
	};

	class IDetailHolder :public ITypeName
	{
	public:
		virtual void Init(TSharedPtr<FDetailOptions> Options) = 0;
		virtual void SetDetailInfo(TSharedPtr<FDetailInfo> Info) = 0;
		virtual TSharedPtr<SWidget> GetWidget() = 0;
	};
}