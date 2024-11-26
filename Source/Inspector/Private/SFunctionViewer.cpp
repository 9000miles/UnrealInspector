
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
	FunctionDetailHolder = Manager.Create<FFunctionDetailHolder>(Options);
	TSharedPtr<SWidget> FunctionDetailsViewer = FunctionDetailHolder->GetWidget();

	ChildSlot
		[
			SNew(SSplitter)
				+ SSplitter::Slot()
				.Value(1.45f)
				[
					SAssignNew(FunctionGroups, SVerticalBox)
				]
				+ SSplitter::Slot()
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						[
							FunctionDetailsViewer.ToSharedRef()
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
	TSharedRef<SToolTip> ToolTip = SNew(SToolTip)
		.BorderImage(FCoreStyle::Get().GetBrush("PopupText.Background"))
		.TextMargin(FMargin(15.0f, 15.0f))
		[
			SNew(STextBlock).Text(Item->GetFunctionSignature())
		]
		;

	TSharedRef<SWidget> Widget = SNew(STextBlock)
		.Text(Item->GetFunctionName())
		;
	Widget->SetToolTip(ToolTip);

	return SNew(STableRow<TSharedPtr<FFunctionHolder>>, OwnerTable)
		[
			Widget
		];
}

void SFunctionViewer::OnSelectionChanged(TSharedPtr<FFunctionHolder> NewSelection, ESelectInfo::Type SelectInfo)
{
	TSharedPtr<FDetailInfo> ParameterDetailInfo = MakeShared<FDetailInfo>();
	FunctionDetailHolder->SetFunction(NewSelection->Function);
}

FReply SFunctionViewer::OnExecute()
{
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

void FFunctionDetailHolder::Init(TSharedPtr<FDetailOptions> Options)
{
}

void FFunctionDetailHolder::SetDetailInfo(TSharedPtr<FDetailInfo> Info)
{
	if (DetailInfo.IsValid())
	{
		DetailInfo->CategoryList->Sort();
	}

	if (DetailViewer.IsValid())
	{
		DetailViewer->SetDetailInfo(Info);
		DetailViewer->ExpandAll(true);
	}
}

TSharedPtr<SWidget> FFunctionDetailHolder::GetWidget()
{
	if (DetailViewer.IsValid())
		return DetailViewer;

	DetailViewer = SNew(SDetailViewer)
		.DetailInfo(DetailInfo)
		;

	return DetailViewer;
}

void FFunctionDetailHolder::SetFunction(UFunction* InFunction)
{
	Function = InFunction;

	if (Function == nullptr)
	{
		DetailInfo.Reset();
		DetailInfo = nullptr;
	}
	else
	{
		DetailInfo = MakeShareable(new FDetailInfo());
		DetailInfo->Name = Function->GetName();
		DetailInfo->DisplayName = Function->GetName();
		DetailInfo->Description = Function->GetName();
		//DetailInfo->Commander = MakeShareable(new FUObjectDetailCommander());

		TSharedPtr<FCategoryInfo> ParameterCategoryInfo = MakeShareable(new FCategoryInfo());
		ParameterCategoryInfo->Name = TEXT("Parameters");

		TSharedPtr<FCategoryInfo> ReturnCategoryInfo = MakeShareable(new FCategoryInfo());
		ReturnCategoryInfo->Name = TEXT("Return");

		for (TFieldIterator<FProperty> It(Function); It; ++It)
		{
			FProperty* Property = *It;
			if (Property->HasAnyPropertyFlags(CPF_Parm))
			{
				if (!Property->HasAnyPropertyFlags(CPF_ReturnParm))
				{
					// Parameter
					TSharedPtr<FPropertyInfo> PropertyInfo = MakeShareable(new FPropertyInfo());
					PropertyInfo->Name = Property->GetName();
					PropertyInfo->Type = PROPERTY::FUEPropertyHelper::GetPropertyType(Property);
					PropertyInfo->Category = ParameterCategoryInfo->Name;
					PropertyInfo->Executor = MakeShareable(new PROPERTY::FUObjectParameterExecutor(Function, Property, Function, nullptr));
					PropertyInfo->Metadata = MakeShareable(new PROPERTY::FUEPropertyMetadata(Property));

					ParameterCategoryInfo->Add(PropertyInfo);
				}
				else
				{
					// Return Parameter
					TSharedPtr<FPropertyInfo> PropertyInfo = MakeShareable(new FPropertyInfo());
					PropertyInfo->Name = TEXT("Return");
					PropertyInfo->Type = PROPERTY::FUEPropertyHelper::GetPropertyType(Property);
					PropertyInfo->Category = ReturnCategoryInfo->Name;
					PropertyInfo->Executor = MakeShareable(new PROPERTY::FUObjectParameterExecutor(Function, Property, Function, nullptr));
					PropertyInfo->Metadata = MakeShareable(new PROPERTY::FUEPropertyMetadata(Property));

					ReturnCategoryInfo->Add(PropertyInfo);
				}
			}
		}

		DetailInfo->CategoryList->Add(ParameterCategoryInfo);
		DetailInfo->CategoryList->Add(ReturnCategoryInfo);
	}

	SetDetailInfo(DetailInfo);
}