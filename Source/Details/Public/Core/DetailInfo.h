// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Interface/ITypeName.h"
#include "Interface/IJsonable.h"

namespace DETAILS_VIEWER { class FPropertyTreeNode; }

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

	class DETAILSVIEWER_API Factory
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
	class DETAILSVIEWER_API IDetailMaker :public IJsonable, public ITypeName
	{
	public:
		virtual ~IDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) = 0;
	};

	class DETAILSVIEWER_API FDetailMaker : public IDetailMaker
	{
	public:
		virtual ~FDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) override;

		FString GetTypeName() override;

	};

	class DETAILSVIEWER_API FCustomDetailMaker :public IDetailMaker
	{
	public:
		virtual ~FCustomDetailMaker() {}
		virtual TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) override;

		FString GetTypeName() override;

	};

	class DETAILSVIEWER_API ICopier
	{
	public:
		virtual ~ICopier() {}
		virtual const FString Execute() = 0;
	};

	class DETAILSVIEWER_API IPaster
	{
	public:
		virtual ~IPaster() {}
		virtual void Execute(const FString String) = 0;
	};

	/**
	 * 细节面板指挥官
	 * 负责创建细节面板，整个面板的复制和粘贴
	 */
	class DETAILSVIEWER_API IDetailCommander :public IJsonable, public ITypeName
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

	class DETAILSVIEWER_API ICategoryExecutor :public IJsonable
	{
	public:
		virtual void Execute() = 0;
	};

	namespace PROPERTY
	{
		enum EPropertyChangeAction
		{
			Unspecified,
			ArrayAdd,
			ArrayRemove,
			ArrayClear,
			Duplicate,
		};

		struct FEnumValue
		{
			FEnumValue(uint8 InValue = 0) :Value(InValue) {}
			uint8 Value = 0;
		};

		class DETAILSVIEWER_API IPropertyAccessor :public IJsonable
		{
		public:
			virtual void Set(const void* In, int32 Size) = 0;
			virtual bool Get(void* Out, int32 Size) = 0;
			virtual void Reset() = 0;
			virtual void OnPropertyChanged(FString MemberName, FString InnerName, EPropertyChangeAction Action) = 0;
		};

		class DETAILSVIEWER_API IConditionEvaluator :public IJsonable
		{
		public:
			enum EType
			{
				None,
				Bool,//bool求值
				Equal,//值相等
				NotEqual,//值不相等
				Less,//小于
				Greater,//大于
				LessEqual,//小于等于
				GreaterEqual,//大于等于
			};

			void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
			TSharedPtr<FJsonObject> ToJson() override;

		protected:
			bool EvaluateCondition(const FString& Expression)
			{
				bool bIsAndExpression = Expression.Contains(TEXT("&&"));
				const FString SplitSymbol = bIsAndExpression ? TEXT("&&") : TEXT("||");

				TArray<FString> Conditions;
				Expression.ParseIntoArray(Conditions, *SplitSymbol, true);
				if (Conditions.IsEmpty()) Conditions.Add(Expression);

				for (FString& Condition : Conditions)
				{
					if (Condition.IsEmpty()) continue;

					Condition.TrimStartAndEndInline();

					bool bNotValue = false;
					if (Condition.StartsWith(TEXT("!"))) {
						bNotValue = true;
						Condition = Condition.RightChop(1).TrimStart();
					}

					EType ComparisionType = EType::Bool;
					if (Condition.Equals(TEXT("true"), ESearchCase::IgnoreCase)) return true;
					else if (Condition.Equals(TEXT("false"), ESearchCase::IgnoreCase)) { if (bIsAndExpression) return false; }
					else if (Condition.Contains(TEXT("=="))) ComparisionType = EType::Equal;
					else if (Condition.Contains(TEXT("!="))) ComparisionType = EType::NotEqual;
					else if (Condition.Contains(TEXT("<="))) ComparisionType = EType::LessEqual;
					else if (Condition.Contains(TEXT(">="))) ComparisionType = EType::GreaterEqual;
					else if (Condition.Contains(TEXT(">"))) ComparisionType = EType::Greater;
					else if (Condition.Contains(TEXT("<"))) ComparisionType = EType::Less;
					else if (Condition.Contains(TEXT("="))) ComparisionType = EType::Equal;

					bool Result = EvaluateSingleCondition(Condition, ComparisionType);
					if (bNotValue) Result = !Result;

					// && 只要有一个为false，就返回false
					if (bIsAndExpression) { if (!Result) return false; }
					// || 只要有一个为true，就返回true
					else { if (Result) return true; }
				}

				return bIsAndExpression ? true : false;
			}

		public:
			virtual bool IsCanEditable() = 0;
			virtual bool IsCanVisible() = 0;
			virtual bool EvaluateSingleCondition(const FString& Condition, EType Type) = 0;
		};

		class DETAILSVIEWER_API IWidgetMaker :public IJsonable
		{
		public:
			virtual TSharedRef<SWidget> MakeWidget(TSharedPtr<FTreeNode> Node) = 0;
		};

		class DETAILSVIEWER_API IExecutor :public IJsonable
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
			TSharedPtr<IConditionEvaluator> Condition;
			TSharedPtr<IWidgetMaker> WidgetMaker;
			TSharedPtr<ICopier> Copier;
			TSharedPtr<IPaster> Paster;
		};

		class DETAILSVIEWER_API FMetadata :public IJsonable
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
						else if constexpr (std::is_same_v<T, FName>)
							Element = *Value->AsString();
						else if constexpr (std::is_same_v<T, FText>)
							Element = FText::FromString(Value->AsString());
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
			FName Get(const FString& Key) { return *GetValue<FString>(Key); }

			template<>
			bool Get(const FString& Key) { return GetValue<bool>(Key); }

			template<>
			TArray<FString> Get(const FString& Key) { return GetArray<FString>(Key); }

			template<>
			TArray<FText> Get(const FString& Key) { return GetArray<FText>(Key); }

			template<>
			TArray<FName> Get(const FString& Key) { return GetArray<FName>(Key); }

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

			bool Has(const FString& Key) { return Metadata->HasField(Key); }

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





	class DETAILSVIEWER_API FPropertyInfo :public TSharedFromThis<FPropertyInfo>, public IJsonable
	{
	public:
		virtual ~FPropertyInfo()
		{
		}
		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		FString Copy();
		void Paste(const FString& String);

	public:
		void Enumerate(TFunction<void(TSharedPtr<FPropertyInfo>)> Func);

		void MakeTreeNode(TSharedPtr<FTreeNode> Node);
	public:
		FString Name;
		FString Description;
		FString DisplayName;
		FString Type;
		FString Category;
		bool Advanced;
		TSharedPtr<PROPERTY::IExecutor> Executor;
		TSharedPtr<PROPERTY::FMetadata> Metadata;
		TArray<TSharedPtr<FPropertyInfo>> Children;

	};

	class DETAILSVIEWER_API FPropertyList :public IJsonable
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

		FString Copy();
		void Paste(const FString& String);

		FString ConvertJsonValueToString(TSharedPtr<FJsonValue> JsonValue);
	private:
		TArray<TSharedPtr<FPropertyInfo>> Parameters;
	};

	class DETAILSVIEWER_API FCategoryInfo :public IJsonable
	{
	public:
		FCategoryInfo();
		virtual ~FCategoryInfo();

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override;
		TSharedPtr<FJsonObject> ToJson() override;

		void Add(TSharedPtr<FPropertyInfo> Parameter);
		virtual void Sort();

		FString Copy();
		void Paste(const FString& String);

	public:
		FString Name;
		FString Description;
		FString DisplayName;
		TSharedPtr<ICategoryExecutor> CategoryExecutor;
		TSharedPtr<FPropertyList> PropertyList;
	};

	class DETAILSVIEWER_API FCategoryList :public IJsonable
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
	class DETAILSVIEWER_API FDetailInfo :public IJsonable
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

	class DETAILSVIEWER_API IDetailHolder :public ITypeName
	{
	public:
		virtual void Init(TSharedPtr<FDetailOptions> Options) = 0;
		virtual void SetDetailInfo(TSharedPtr<FDetailInfo> Info) = 0;
		virtual TSharedPtr<SWidget> GetWidget() = 0;
	};
}