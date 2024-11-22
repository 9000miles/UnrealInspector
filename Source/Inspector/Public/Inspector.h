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

			return TArray<TSharedPtr<FUObjectHolder>>();
		}
	};

	class FClassifyOuter : public IClassifyHandler
	{
		TArray<TSharedPtr<FUObjectHolder>> Classify(TArray<TSharedPtr<FUObjectHolder>>& Array) override
		{
			TArray<TSharedPtr<FUObjectHolder>> OuterObjects;

			for (int32 i = 0; i < Array.Num(); i++)
			{
				TSharedPtr<FUObjectHolder> ObjectHolder = Array[i];
				if (!ObjectHolder->IsValid()) continue;

				UObject* Outer = ObjectHolder->GetObject()->GetOuter();
				if (!Outer)
				{
					OuterObjects.Add(ObjectHolder);
					continue;
				}

				uint32 ObjectIndex = Outer->GetUniqueID();
				TSharedPtr<FUObjectHolder> OuterObjectHolder = FUObjectCollector::Get().GetObject(ObjectIndex);
				if (OuterObjectHolder.IsValid())
				{
					OuterObjectHolder->GetChildren().Add(ObjectHolder);
				}
			}

			OuterObjects.Sort([](const TSharedPtr<FUObjectHolder>& A, const TSharedPtr<FUObjectHolder>& B)
				{
					return A->GetName() < B->GetName();
				});

			for (auto& ObjectHolder : OuterObjects)
			{
				ObjectHolder->Sort();
			}

			return OuterObjects;
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
}

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
