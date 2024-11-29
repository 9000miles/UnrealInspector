// Fill out your copyright notice in the Description page of Project Settings.


#include "Creater/EnumCreater.h"
#include "Widget/SPropertyWidget.h"

namespace DETAILS_VIEWER
{

	void SPropertyWidgetEnum::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);

		EnumNames = PropertyInfo->Metadata->Get<TArray<FName>>(TEXT("EnumNames"));

		const FName InitialValue = EnumNames[0];

		ChildSlot
			[
				SNew(SComboBox<FName>)
					.OptionsSource(&EnumNames)
					.OnGenerateWidget(this, &SPropertyWidgetEnum::OnGenerateComboWidget)
					.OnSelectionChanged(this, &SPropertyWidgetEnum::OnSelectionChanged)
					.InitiallySelectedItem(InitialValue)
					[
						SNew(STextBlock)
							.Text(this, &SPropertyWidgetEnum::GetPropertyValue)
					]
			];
	}

	TSharedRef<SWidget> SPropertyWidgetEnum::OnGenerateComboWidget(FName InComboString)
	{
		return SNew(STextBlock)
			.Text(FText::FromName(InComboString));
	}

	void SPropertyWidgetEnum::OnSelectionChanged(FName InSelectedItem, ESelectInfo::Type SelectInfo)
	{
		uint8 Value = EnumNames.IndexOfByKey(InSelectedItem);
		GetAccessor()->Set(&Value, sizeof(decltype(Value)));
	}

	FText SPropertyWidgetEnum::GetPropertyValue() const
	{
		uint8 Value = 0;
		if (!GetAccessor()->Get(&Value, sizeof(decltype(Value))))
			Value = 0;

		return FText::FromName(EnumNames[Value]);
	}


	FWidgetCreaterEnum::FWidgetCreaterEnum()
	{
	}

	TSharedPtr<SWidget> FWidgetCreaterEnum::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		TSharedPtr<FPropertyTreeNode> PropertyTreeNode = StaticCastSharedPtr<FPropertyTreeNode>(TreeNode);
		return SNew(SPropertyWidgetEnum, PropertyTreeNode->PropertyInfo);
	}

	TArray<FString> FWidgetCreaterEnum::SupportTypes()
	{
		return { TEXT("Enum") ,TEXT("enum") };
	}

}