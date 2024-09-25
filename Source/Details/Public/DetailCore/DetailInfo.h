// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

namespace DetailsViewer
{
	class IJsonable
	{
	public:
		virtual void FromJSON(TSharedPtr<FJsonObject> JsonObject)
		{
		}

		void FromJsonString(const FString& JsonString)
		{
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
			TSharedPtr<FJsonObject> JsonObject;
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
				FromJSON(JsonObject);
		}
		virtual TSharedPtr<FJsonObject> ToJson()
		{
			return MakeShared<FJsonObject>();
		}

		FString ToJsonString()
		{
			TSharedPtr<FJsonObject> JsonObject = ToJson();
			FString JsonString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			return JsonString;
		}
	};

	class ITypeName
	{
	public:
		virtual FString GetTypeName() = 0;
	};

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
	 * 细节面板构建器
	 */
	class IDetailMaker :public ITypeName
	{
	public:
		virtual ~IDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget() = 0;
	};

	class FDetailMaker : public IDetailMaker
	{
	public:
		virtual ~FDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget() override;

		FString GetTypeName() override;

	};

	class FCustomDetailMaker :public IDetailMaker
	{
	public:
		virtual ~FCustomDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget() override;

		FString GetTypeName() override;

	};

	class ICopyExecutor : public IExecutor
	{
	public:
		virtual ~ICopyExecutor() {}
		virtual void Execute() = 0;
	};

	class IPasteExecutor : public IExecutor
	{
	public:
		virtual ~IPasteExecutor() {}
		virtual void Execute() = 0;
	};

	class IDetailExecutor :public IJsonable
	{
	public:
		virtual ~IDetailExecutor() {}
		void FromJSON(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

	public:
		TSharedPtr<IDetailMaker> DetailMaker;
		TSharedPtr<ICopyExecutor> CopyExecutor;
		TSharedPtr<IPasteExecutor> PasteExecutor;


	};

	class ICategoryExecutor
	{
	public:
		virtual void Execute() = 0;
	};

	namespace Parameter
	{
		class ISetter
		{
		public:
			virtual void Set(FString Value) = 0;
		};
		class IGetter
		{
		public:
			virtual FString Get() = 0;
		};
		class IEditable
		{
		public:
			virtual bool CanEdit() = 0;
		};
		class IVisible
		{
		public:
			virtual bool CanVisible() = 0;
		};
		class IDefaultGetter
		{
		public:
			virtual FString GetDefault() = 0;
		};
		class IWidgetMaker
		{
		public:
			virtual TSharedRef<SWidget> MakeWidget(const FString& Value) = 0;
		};

		class IExecutor
		{
		public:
			TSharedPtr<ISetter> Setter;
			TSharedPtr<IGetter> Getter;
			TSharedPtr<IEditable> Editable;
			TSharedPtr<IVisible> Visible;
			TSharedPtr<IDefaultGetter> DefaultGetter;
			TSharedPtr<IWidgetMaker> WidgetMaker;
			TSharedPtr<ICopyExecutor> CopyExecutor;
			TSharedPtr<IPasteExecutor> PasteExecutor;
		};

		class IMetadata
		{
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





	class IParameterInfo
	{
	public:
		FString Name;
		FString Description;
		FString DisplayName;
		FString Type;
		FString Category;
		bool Advanced;
		TSharedPtr<Parameter::IExecutor> ParameterExecutor;
		TSharedPtr<Parameter::IMetadata> ParameterMetadata;
	};

	class ICategoryInfo :public IJsonable
	{
	public:
		virtual ~ICategoryInfo()
		{
			CategoryExecutor.Reset();
			CategoryExecutor = nullptr;

			Parameters.Empty();
		}
	public:
		FString Name;
		FString Description;
		FString DisplayName;
		TSharedPtr<ICategoryExecutor> CategoryExecutor;
		TArray<IParameterInfo> Parameters;
	};

	/**
	 * $Comment$
	 */
	class IDetailInfo :public IJsonable
	{
	public:
		virtual ~IDetailInfo()
		{
			DetailExecutor.Reset();
			DetailExecutor = nullptr;

			CategoryList.Empty();
		}

		void FromJSON(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

	public:
		FString Name;
		FString Description;
		FString DisplayName;
		TSharedPtr<IDetailExecutor> DetailExecutor;
		TArray<TSharedPtr<ICategoryInfo>> CategoryList;


	};
}