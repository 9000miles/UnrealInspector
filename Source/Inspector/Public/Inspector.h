// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Views/SListView.h"

namespace UObjectCollector { class FUObjectHolder; }
namespace UObjectCollector { class FUObjectInfo; }
namespace DETAILS_VIEWER { class FUObjectDetailHolder; }

using namespace UObjectCollector;
using namespace DETAILS_VIEWER;

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

private:
	TSharedPtr<FUObjectDetailHolder> DetailHolder;
	TSharedPtr < SListView<TSharedPtr<FUObjectHolder>>> ObjectListView;
	TArray<TSharedPtr<FUObjectHolder>> ObjectList;
};
