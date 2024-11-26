// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DetailInfo.h"

namespace UOBJECT_COLLECTOR { class FFunctionHolder; }


class FFunctionDetailHolder :public IDetailHolder
{
	IMPLEMENT_ITYPENAME(FFunctionDetailHolder)
public:
	void Init(TSharedPtr<FDetailOptions> Options) override;
	void SetDetailInfo(TSharedPtr<FDetailInfo> Info) override;
	TSharedPtr<SWidget> GetWidget() override;

	void SetFunction(UFunction* InFunction);

	FString GetPropertyType(UE_Property* Property);
private:
	UFunction* Function;
	TSharedPtr<FDetailInfo> DetailInfo;
	TSharedPtr<SDetailViewer> DetailViewer;
};
/**
 * Comment
 */
class SFunctionViewer : public SCompoundWidget
{
	enum EFuncitonAccess
	{
		Static,
		Public,
		Protected,
		Private,
	};
public:
	SLATE_BEGIN_ARGS(SFunctionViewer)
		{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void SetObject(UObject* InObject);
	TSharedRef<SWidget> MakeFunctionGroup(EFuncitonAccess Access);
	TSharedRef<SWidget> MakeFunctionList(EFuncitonAccess Access);

	TSharedRef<class ITableRow> GenerateRowWidget(TSharedPtr<FFunctionHolder> Item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnSelectionChanged(TSharedPtr<FFunctionHolder> NewSelection, ESelectInfo::Type SelectInfo);
	FReply OnExecute();

private:
	TWeakObjectPtr<UObject> Object;
	TSharedPtr<SVerticalBox> FunctionGroups;

	TArray<TSharedPtr<FFunctionHolder>> StaticFunctions;
	TArray<TSharedPtr<FFunctionHolder>> PublicFunctions;
	TArray<TSharedPtr<FFunctionHolder>> ProtectedFunctions;
	TArray<TSharedPtr<FFunctionHolder>> PrivateFunctions;

	TArray<TSharedPtr<FFunctionHolder>> FunctionList;

	TSharedPtr< FFunctionDetailHolder> FunctionDetailHolder;
};