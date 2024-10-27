#include "Creater/GuidCreater.h"
namespace DETAILS_VIEWER
{
#define LOCTEXT_NAMESPACE "SPropertyWidgetGuid"
	void SPropertyWidgetGuid::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);

		ChildSlot[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SEditableTextBox)
						.Text(this, &SPropertyWidgetGuid::GetPropertyValue)
						.IsReadOnly(true)
						//.OnTextCommitted(this, &SPropertyWidgetGuid::OnTextCommitted)
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

	}

	FText SPropertyWidgetGuid::GetPropertyValue() const
	{
		FGuid Guid;
		GetExecutor()->Accessor->Get(Guid);
		return FText::FromString(Guid.ToString());
	}

	FReply SPropertyWidgetGuid::OnButtonClicked()
	{
		GetExecutor()->Accessor->Set(FGuid::NewGuid());
		return FReply::Handled();
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

