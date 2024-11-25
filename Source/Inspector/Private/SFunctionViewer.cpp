
// Fill out your copyright notice in the Description page of Project Settings.

#include "SFunctionViewer.h"
#include "SlateOptMacros.h"
#include "UObjectHolder.h"
#include "Core/DetailDefine.h"
#include "Detail/UObjectDetail.h"

#define LOCTEXT_NAMESPACE "SUObjectFunctions"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SFunctionViewer::Construct(const FArguments& InArgs)
{
	FDetialManager& Manager = FDetialManager::Get();
	TSharedPtr<FDetailOptions> Options = MakeShared<FDetailOptions>();
	ParameterDetailHolder = Manager.Create<FUObjectDetailHolder>(Options);
	TSharedPtr<SWidget> ParameterDetailsViewer = ParameterDetailHolder->GetWidget();

	ReturnDetailHolder = Manager.Create<FUObjectDetailHolder>(Options);
	TSharedPtr<SWidget> ReturnDetailsViewer = ReturnDetailHolder->GetWidget();

	ChildSlot
		[
			SNew(SSplitter)
				+ SSplitter::Slot()
				.Value(0.75f)
				[
					SAssignNew(FunctionGroups, SVerticalBox)
				]
				+ SSplitter::Slot()
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						[
							SNew(SSplitter)
								+ SSplitter::Slot()
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot().FillHeight(0.1f).HAlign(HAlign_Center).VAlign(VAlign_Center)
										[
											SNew(STextBlock).Text(LOCTEXT("Parameters", "Parameters"))
										]
										+ SVerticalBox::Slot()
										[
											ParameterDetailsViewer.ToSharedRef()
										]
								]
								+ SSplitter::Slot()
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot().FillHeight(0.1f).HAlign(HAlign_Center).VAlign(VAlign_Center)
										[
											SNew(STextBlock).Text(LOCTEXT("Return", "Return"))
										]
										+ SVerticalBox::Slot()
										[
											ReturnDetailsViewer.ToSharedRef()
										]
								]
						]
						+ SVerticalBox::Slot()
						.FillHeight(0.1f)
						//.AutoHeight()
						[
							SNew(SBox)
								.HAlign(HAlign_Fill).VAlign(VAlign_Fill)
								//.HAlign(HAlign_Center).VAlign(VAlign_Center)
								[
									SNew(SButton)
										//.Text(LOCTEXT("Execute", "Execute"))
										.OnClicked(this, &SFunctionViewer::OnExecute)
										//.IsEnabled(this, &SFunctionViewer::CanExecute)
										.HAlign(HAlign_Center).VAlign(VAlign_Center)
										[
											SNew(STextBlock)
												.Text(LOCTEXT("Execute", "Execute"))
										]
								]
						]
				]
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SFunctionViewer::SetObject(UObject* InObject)
{
	Object = InObject;
	if (!Object.IsValid()) return;

	FunctionGroups->ClearChildren();
	StaticFunctions.Empty();
	PublicFunctions.Empty();
	ProtectedFunctions.Empty();
	PrivateFunctions.Empty();

	for (TFieldIterator<UFunction> FuncIt(Object->GetClass(), EFieldIteratorFlags::IncludeSuper); FuncIt; ++FuncIt)
	{
		UFunction* Function = *FuncIt;
		if (Function->HasAnyFunctionFlags(EFunctionFlags::FUNC_Static))
			StaticFunctions.Add(MakeShared<FFunctionHolder>(Function));
		else if (Function->HasAnyFunctionFlags(EFunctionFlags::FUNC_Public))
			PublicFunctions.Add(MakeShared<FFunctionHolder>(Function));
		else if (Function->HasAnyFunctionFlags(EFunctionFlags::FUNC_Protected))
			ProtectedFunctions.Add(MakeShared<FFunctionHolder>(Function));
		else if (Function->HasAnyFunctionFlags(EFunctionFlags::FUNC_Private))
			PrivateFunctions.Add(MakeShared<FFunctionHolder>(Function));
	}

	FunctionGroups->AddSlot()[MakeFunctionGroup(EFuncitonAccess::Static)];
	FunctionGroups->AddSlot()[MakeFunctionGroup(EFuncitonAccess::Public)];
	FunctionGroups->AddSlot()[MakeFunctionGroup(EFuncitonAccess::Protected)];
	FunctionGroups->AddSlot()[MakeFunctionGroup(EFuncitonAccess::Private)];
}

TSharedRef<SWidget> SFunctionViewer::MakeFunctionGroup(EFuncitonAccess Access)
{
	FText FunctionListTitle;
	switch (Access)
	{
	case EFuncitonAccess::Static: FunctionListTitle = LOCTEXT("StaticFunctionList", "Static"); break;
	case EFuncitonAccess::Public: FunctionListTitle = LOCTEXT("PublicFunctionList", "Public"); break;
	case EFuncitonAccess::Protected: FunctionListTitle = LOCTEXT("ProtectedFunctionList", "Protected"); break;
	case EFuncitonAccess::Private: FunctionListTitle = LOCTEXT("PrivateFunctionList", "Private"); break;
	default: FunctionListTitle = LOCTEXT("UnknownFunctionList", "Unknown"); break;
	}

	return SNew(SExpandableArea)
		.AreaTitle(FunctionListTitle)
		.BodyContent()
		[
			MakeFunctionList(Access)
		]
		;
}

TSharedRef<SWidget> SFunctionViewer::MakeFunctionList(EFuncitonAccess Access)
{
	if (!Object.IsValid()) return SNullWidget::NullWidget;

	TSharedPtr<SListView<TSharedPtr<FFunctionHolder>>> FunctionListView;

	switch (Access)
	{
	case EFuncitonAccess::Static:
		return SNew(SListView<TSharedPtr<FFunctionHolder>>)
			.ListItemsSource(&StaticFunctions)
			.OnGenerateRow(this, &SFunctionViewer::GenerateRowWidget)
			.OnSelectionChanged(this, &SFunctionViewer::OnSelectionChanged)
			.SelectionMode(ESelectionMode::Single)
			.ItemHeight(20.0f)
			;
	case EFuncitonAccess::Public:
		return SNew(SListView<TSharedPtr<FFunctionHolder>>)
			.ListItemsSource(&PublicFunctions)
			.OnGenerateRow(this, &SFunctionViewer::GenerateRowWidget)
			.OnSelectionChanged(this, &SFunctionViewer::OnSelectionChanged)
			.SelectionMode(ESelectionMode::Single)
			.ItemHeight(50.0f)
			;
	case EFuncitonAccess::Protected:
		return SNew(SListView<TSharedPtr<FFunctionHolder>>)
			.ListItemsSource(&ProtectedFunctions)
			.OnGenerateRow(this, &SFunctionViewer::GenerateRowWidget)
			.OnSelectionChanged(this, &SFunctionViewer::OnSelectionChanged)
			.SelectionMode(ESelectionMode::Single)
			.ItemHeight(20.0f)
			;
	case EFuncitonAccess::Private:
		return SNew(SListView<TSharedPtr<FFunctionHolder>>)
			.ListItemsSource(&PrivateFunctions)
			.OnGenerateRow(this, &SFunctionViewer::GenerateRowWidget)
			.OnSelectionChanged(this, &SFunctionViewer::OnSelectionChanged)
			.SelectionMode(ESelectionMode::Single)
			.ItemHeight(20.0f)
			;
	}

	return SNullWidget::NullWidget;
}

TSharedRef<class ITableRow> SFunctionViewer::GenerateRowWidget(TSharedPtr<FFunctionHolder> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FFunctionHolder>>, OwnerTable)
		[
			SNew(STextBlock)
				.Text(Item->GetFunctionName())
				.ToolTipText(Item->GetFunctionSignature())
		];
}

void SFunctionViewer::OnSelectionChanged(TSharedPtr<FFunctionHolder> NewSelection, ESelectInfo::Type SelectInfo)
{

}

FReply SFunctionViewer::OnExecute()
{
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
