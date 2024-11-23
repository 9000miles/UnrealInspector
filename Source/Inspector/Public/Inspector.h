// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Views/SListView.h"
#include "UObjectHolder.h"
#include "UObjectCollector.h"


namespace UObjectCollector { class FUObjectHolder; }
namespace DETAILS_VIEWER { class FUObjectDetailHolder; }


using namespace UObjectCollector;
using namespace DETAILS_VIEWER;

namespace INSPECTOR
{
	enum EClassifyType
	{
		Package,
		Class,
		Outer,
	};

	class IClassifyHandler
	{
	public:
		virtual TArray<TSharedPtr<FUObjectHolder>> Classify(TArray<TSharedPtr<FUObjectHolder>>& Array) = 0;
		virtual void Sort(TArray<TSharedPtr<FUObjectHolder>>& Array)
		{
			Array.Sort([](const TSharedPtr<FUObjectHolder>& A, const TSharedPtr<FUObjectHolder>& B)
				{
					return A->GetName() < B->GetName();
				});

			for (auto& ObjectHolder : Array)
			{
				ObjectHolder->Sort();
			}
		}
		virtual void ClearChildren()
		{
			FUObjectCollector::Get().ClearChildren();
		}
	};

	class FClassifyPackage : public IClassifyHandler
	{
		TArray<TSharedPtr<FUObjectHolder>> Classify(TArray<TSharedPtr<FUObjectHolder>>& Array) override
		{
			return TArray<TSharedPtr<FUObjectHolder>>();
		}
	};

	class FClassifyClass : public IClassifyHandler
	{
		TArray<TSharedPtr<FUObjectHolder>> Classify(TArray<TSharedPtr<FUObjectHolder>>& Array) override
		{
			ClearChildren();

			TArray<TSharedPtr<FUObjectHolder>> Result;
			UClass* CoreUObjectClass = LoadClass<UClass>(nullptr, TEXT("/Script/CoreUObject.Class"));

			for (int32 i = 0; i < Array.Num(); i++)
			{
				TSharedPtr<FUObjectHolder> Object_Holder = Array[i];
				if (!Object_Holder->IsValid()) continue;

				UClass* ObjectClass = Object_Holder->GetObject()->GetClass();
				UClass* ObjectSuperClass = ObjectClass->GetSuperClass();
				if (!ObjectSuperClass)
				{
					uint32 ObjectIndex = ObjectClass->GetUniqueID();
					TSharedPtr<FUObjectHolder> RootClassObjectHolder = FUObjectCollector::Get().GetObject(ObjectIndex);
					if (!Result.Contains(RootClassObjectHolder))
						Result.Add(RootClassObjectHolder);
				}
				else
				{
					uint32 SuperObjectIndex = ObjectSuperClass->GetUniqueID();
					TSharedPtr<FUObjectHolder> ObjectSuperClass_Holder = FUObjectCollector::Get().GetObject(SuperObjectIndex);

					uint32 ClassObjectIndex = ObjectClass->GetUniqueID();
					TSharedPtr<FUObjectHolder> ObjectClass_Holder = FUObjectCollector::Get().GetObject(ClassObjectIndex);

					if (!ObjectSuperClass_Holder->GetChildren().Contains(ObjectClass_Holder))
						ObjectSuperClass_Holder->GetChildren().Add(ObjectClass_Holder);

					if (CoreUObjectClass == Object_Holder->GetObject())
						continue;

					UClass* ObjectClassClass = ObjectClass_Holder->GetObject()->GetClass();
					//if (ObjectClassClass == CoreUObjectClass)
					//	ObjectClass_Holder->GetChildren().Add(Object_Holder);
				}
			}

			Sort(Result);

			return Result;
		};
	};

	class FClassifyOuter : public IClassifyHandler
	{
		TArray<TSharedPtr<FUObjectHolder>> Classify(TArray<TSharedPtr<FUObjectHolder>>& Array) override
		{
			ClearChildren();

			TArray<TSharedPtr<FUObjectHolder>> Result;

			for (int32 i = 0; i < Array.Num(); i++)
			{
				TSharedPtr<FUObjectHolder> ObjectHolder = Array[i];
				if (!ObjectHolder->IsValid()) continue;

				UObject* Outer = ObjectHolder->GetObject()->GetOuter();
				if (!Outer)
				{
					Result.Add(ObjectHolder);
					continue;
				}

				uint32 ObjectIndex = Outer->GetUniqueID();
				TSharedPtr<FUObjectHolder> OuterObjectHolder = FUObjectCollector::Get().GetObject(ObjectIndex);
				if (OuterObjectHolder.IsValid())
				{
					OuterObjectHolder->GetChildren().Add(ObjectHolder);
				}
			}

			Sort(Result);

			return Result;
		}
	};

	class FUObjectClassify
	{
	public:
		FUObjectClassify(EClassifyType InClassifyType)
			:ClassifyType(InClassifyType)
		{
			switch (ClassifyType)
			{
			case EClassifyType::Class:
				ClassifyHandler = MakeShareable(new FClassifyClass);
				break;
			case EClassifyType::Package:
				ClassifyHandler = MakeShareable(new FClassifyPackage);
				break;
			case EClassifyType::Outer:
				ClassifyHandler = MakeShareable(new FClassifyOuter);
				break;
			}
		}
		FString GetTypeName()
		{
			switch (ClassifyType)
			{
			case INSPECTOR::Package: return TEXT("Package");
			case INSPECTOR::Class: return TEXT("Class");
			case INSPECTOR::Outer: return TEXT("Outer");
			}
			return FString(TEXT("Unknown"));
		}

		TArray<TSharedPtr<FUObjectHolder>> Classify(TArray<TSharedPtr<FUObjectHolder>>& Array)
		{
			if (!ClassifyHandler.IsValid()) return {};

			return ClassifyHandler->Classify(Array);
		}

	public:
		EClassifyType ClassifyType;
		TSharedPtr<IClassifyHandler> ClassifyHandler;
	};
};

using namespace INSPECTOR;

class FInspectorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;

	TSharedRef<class ITableRow> GenerateRowWidget(TSharedPtr<FUObjectHolder> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<SWidget> MakeWidget();
	virtual void ShutdownModule() override;

	void OnObjectAdded(TSharedPtr<FUObjectHolder> ObjectInfo);
	void OnObjectDeleted(TSharedPtr<FUObjectHolder> ObjectInfo);

	void OnSearchTextChanged(const FText& Text);
	EVisibility GetRowVisible(TSharedPtr<FUObjectHolder> Holder) const;

	void OnClassifySelectionChanged(TSharedPtr<FUObjectClassify> NewSelection, ESelectInfo::Type SelectInfo);

	void OnGetChildren(TSharedPtr<FUObjectHolder> Node, TArray<TSharedPtr<FUObjectHolder>>& Children);

private:
	TSharedPtr<FUObjectDetailHolder> DetailHolder;
	TSharedPtr<STreeView<TSharedPtr<FUObjectHolder>>> ObjectTreeView;
	TSharedPtr<SWidget> ObjectInspector;
	TArray<TSharedPtr<FUObjectHolder>> ObjectList;
	TArray<TSharedPtr<FUObjectClassify>> ClassifyList;
};
