#include "Creater/GuidCreater.h"

namespace DETAILS_VIEWER
{
	TArray<FName> GuidFormatNames = {
		TEXT("Digits"),
		TEXT("DigitsLower"),
		TEXT("DigitsWithHyphens"),
		TEXT("DigitsWithHyphensLower"),
		TEXT("DigitsWithHyphensInBraces"),
		TEXT("DigitsWithHyphensInParentheses"),
		TEXT("HexValuesInBraces"),
		TEXT("UniqueObjectGuid"),
		TEXT("Short"),
		TEXT("Base36Encoded")
	};

#define LOCTEXT_NAMESPACE "SPropertyWidgetGuid"
	void SPropertyWidgetGuid::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);

		GuidFormatMap.Add(GuidFormatNames[0], EGuidFormats::Digits);
		GuidFormatMap.Add(GuidFormatNames[1], EGuidFormats::DigitsLower);
		GuidFormatMap.Add(GuidFormatNames[2], EGuidFormats::DigitsWithHyphens);
		GuidFormatMap.Add(GuidFormatNames[3], EGuidFormats::DigitsWithHyphensLower);
		GuidFormatMap.Add(GuidFormatNames[4], EGuidFormats::DigitsWithHyphensInBraces);
		GuidFormatMap.Add(GuidFormatNames[5], EGuidFormats::DigitsWithHyphensInParentheses);
		GuidFormatMap.Add(GuidFormatNames[6], EGuidFormats::HexValuesInBraces);
		GuidFormatMap.Add(GuidFormatNames[7], EGuidFormats::UniqueObjectGuid);
		GuidFormatMap.Add(GuidFormatNames[8], EGuidFormats::Short);
		GuidFormatMap.Add(GuidFormatNames[9], EGuidFormats::Base36Encoded);

		const FName DefaultGuidFormat = GuidFormatNames[0];
		GuidFormat = GuidFormatMap[DefaultGuidFormat];

		ChildSlot[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SEditableTextBox)
						.Text(this, &SPropertyWidgetGuid::GetPropertyValue)
						//.IsReadOnly(true)
						.OnTextCommitted(this, &SPropertyWidgetGuid::OnTextCommitted)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SComboBox<FName>)
						.OptionsSource(&GuidFormatNames)
						.OnGenerateWidget(this, &SPropertyWidgetGuid::OnGenerateComboWidget)
						.OnSelectionChanged(this, &SPropertyWidgetGuid::OnSelectionChanged)
						.InitiallySelectedItem(DefaultGuidFormat)
						[
							OnGenerateComboWidget(DefaultGuidFormat)
						]
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
						.OnClicked(this, &SPropertyWidgetGuid::OnButtonClicked)
						.Text(LOCTEXT("Create", "Create"))
				]
		];
	}

	void SPropertyWidgetGuid::OnTextCommitted(const FText& Text, ETextCommit::Type Type)
	{
		GetAccessor()->Set(FGuid(Text.ToString()));
	}

	FText SPropertyWidgetGuid::GetPropertyValue() const
	{
		FGuid Guid;
		GetAccessor()->Get(Guid);
		return FText::FromString(Guid.ToString(GuidFormat));
	}

	FReply SPropertyWidgetGuid::OnButtonClicked()
	{
		GetAccessor()->Set(FGuid::NewGuid());
		return FReply::Handled();
	}

	void SPropertyWidgetGuid::OnSelectionChanged(FName InSelectedItem, ESelectInfo::Type SelectInfo)
	{
		EGuidFormats* Format = GuidFormatMap.Find(InSelectedItem);

		check(Format);
		if (!Format) return;

		GuidFormat = *Format;
	}

	TSharedRef<SWidget> SPropertyWidgetGuid::OnGenerateComboWidget(FName InComboString)
	{
		return SNew(STextBlock)
			.Text(FText::FromName(InComboString));
	}

	TSharedPtr<SWidget> FWidgetCreaterGuid::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		return MakePropertyWidget<SPropertyWidgetGuid>(TreeNode);
	}

	TArray<FString> FWidgetCreaterGuid::SupportTypes()
	{
		return { TEXT("FGuid"), TEXT("Guid") };
	}
#undef LOCTEXT_NAMESPACE
}

