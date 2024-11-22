// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Views/SListView.h"

namespace UObjectCollector { class FUObjectHolder; }
namespace UObjectCollector { class FUObjectHolder; }
namespace DETAILS_VIEWER { class FUObjectDetailHolder; }


namespace INSPECTOR
{
	enum EClassifyType
	{
		Package,
		Class,
		Outer,
	};

	class FUObjectClassify
	{
	public:
		FUObjectClassify(EClassifyType InClassifyType)
			:ClassifyType(InClassifyType)
		{}
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
	public:
		EClassifyType ClassifyType;
	};
}

using namespace UObjectCollector;
using namespace DETAILS_VIEWER;
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
