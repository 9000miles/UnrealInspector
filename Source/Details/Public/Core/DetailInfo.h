// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Interface/ITypeName.h"
#include "Interface/IJsonable.h"

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

	class IDetailExecutor :public IJsonable, public ITypeName
	{
	public:
		virtual ~IDetailExecutor() {}
		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		static FString TypeName() { return TEXT("DetailExecutor"); }
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
		class ISetter :public IJsonable
		{
		public:
			template<typename T>
			void Set(T Value) {};
		};
		class IGetter :public IJsonable
		{
		public:
			template<typename T>
			T Get() {}
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
		class IDefaultGetter :public IGetter
		{
		public:
			virtual FString GetDefault() = 0;
		};
		class IWidgetMaker :public IJsonable
		{
		public:
			virtual TSharedRef<SWidget> MakeWidget(const FString& Value) = 0;
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
			TSharedPtr<ISetter> Setter;
			TSharedPtr<IGetter> Getter;
			TSharedPtr<IEditable> Editable;
			TSharedPtr<IVisible> Visible;
			TSharedPtr<IDefaultGetter> DefaultGetter;
			TSharedPtr<IWidgetMaker> WidgetMaker;
			TSharedPtr<ICopier> CopyExecutor;
			TSharedPtr<IPaster> PasteExecutor;
		};

		class FMetadata :public IJsonable
		{
		public:
			virtual ~FMetadata()
			{
			}
			void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
			TSharedPtr<FJsonObject> ToJson() override;

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





	class IParameterInfo :public IJsonable
	{
	public:
		virtual ~IParameterInfo()
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

	class FParameterList :public IJsonable
	{
	public:
		virtual ~FParameterList()
		{
		}
		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		void Add(TSharedPtr<IParameterInfo> Parameter);
		TSharedPtr<IParameterInfo> Find(const FString& Name);

	private:
		TArray<TSharedPtr<IParameterInfo>> Parameters;
	};

	class ICategoryInfo :public IJsonable
	{
	public:
		ICategoryInfo()
		{
			ParameterList = MakeShared<FParameterList>();
		}
		virtual ~ICategoryInfo();

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		void Add(TSharedPtr<IParameterInfo> Parameter);

	public:
		FString Name;
		FString Description;
		FString DisplayName;
		TSharedPtr<ICategoryExecutor> CategoryExecutor;
		TSharedPtr<FParameterList> ParameterList;
	};

	class FCategoryList :public IJsonable
	{
	public:
		virtual ~FCategoryList()
		{
		}
		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		void Add(TSharedPtr<ICategoryInfo> Category);
		TSharedPtr<ICategoryInfo> Find(const FString& Name);

	private:
		TArray<TSharedPtr<ICategoryInfo>> Categories;
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
		TSharedPtr<IDetailExecutor> DetailExecutor;
		TSharedPtr<FCategoryList> CategoryList;
	};
}