// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailView/SDetailPropertyWidget.h"
#include "SlateOptMacros.h"
#include "DetailView/SPropertyName.h"
#include "DetailCreater/DetailCreater.h"

namespace DetailsViewer
{
	static float SplitterSlotSize = 0.f;

	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
		void SDetailPropertyWidget::Construct(const FArguments& InArgs, TSharedPtr<FDetailTreeNode> DetailNode, bool bOverrideRowWidget, TSharedPtr<SWidget> Widget)
	{
		SDetailTreeItem::Construct(DetailNode);

		OnSplitterSlotResized = InArgs._OnSplitterSlotResized;



		SplitterSlotSize = 0.3f;


		TSharedPtr<SBox> Box = SNew(SBox)
			;

		TSharedPtr<SWidget> CustomDetailRowWidget = Widget;
		if (CustomDetailRowWidget.IsValid() && bOverrideRowWidget)
		{
			Box->SetContent(CustomDetailRowWidget.ToSharedRef());
		}
		else
		{
			//const bool bOverrideRowWidget = DetailNodePtr->OverrideRowWidget();

			TSharedPtr<SWidget> PropertyWidget = Widget.IsValid() ? Widget : DetailNodePtr->GetWidget();
			TSharedPtr<SPropertyName> PropertyName = SNew(SPropertyName, DetailNode);

			TSharedPtr<SWidget> Widget = PropertyWidget;
			if (!PropertyWidget)
			{
				FText Text = FText::FromString(FString::Printf(TEXT("%s Not Implemented"), *DetailNodePtr->GetPropertyHolder()->GetPropertyType()));
				TSharedRef<STextBlock> NotImplementd = SNew(STextBlock)
					.Text(Text)
					;

				Widget = NotImplementd;
			}

			//if (bOverrideRowWidget == false)
			{
				Splitter = SNew(SSplitter)
					.ResizeMode(ESplitterResizeMode::FixedPosition)
					+ SSplitter::Slot()
					.Value(SplitterSlotSize)
					.SizeRule(SSplitter::ESizeRule::FractionOfParent)
					.OnSlotResized(this, &SDetailPropertyWidget::SplitterSlotResized, 0)
					[
						PropertyName.ToSharedRef()
					]
					+ SSplitter::Slot()
					//.Value(SplitterSlotSize)
					.SizeRule(SSplitter::ESizeRule::FractionOfParent)
					.OnSlotResized(this, &SDetailPropertyWidget::SplitterSlotResized, 1)
					[
						SNew(SBorder)
							.BorderImage(FAppStyle::Get().GetBrush("NoBorder"))
							.Padding(FMargin(5, 0, 3, 0))
							[
								SNew(SHorizontalBox)
									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Fill)
									[
										Widget.ToSharedRef()
									]
									+ SHorizontalBox::Slot()
									.Padding(3, 0)
									.AutoWidth()
									[
										SNew(SButton)
											.OnClicked(this, &SDetailPropertyWidget::OnResetClicked)
											[
												SNew(SImage)
											]
									]
							]
					]
					;

				Box->SetContent(Splitter.ToSharedRef());
			}
			//else
			//{
			//	Box->SetContent(PropertyWidget.ToSharedRef());
			//}
		}



		ChildSlot
			[
				Box.ToSharedRef()
			];
	}
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION
		//
		//TSharedPtr<SWidget> SDetailPropertyWidget::CreateCustomDetailRowWidget()
		//{
		//	TSharedPtr<FPropertyProxy> Proxy = DetailNodePtr->GetPropertyProxy();
		//	__Property__* Property = Proxy->GetProperty();
		//	const FString CustomPropertyBuilder = Property->GetMetaData(TEXT("PropertyBuilder"));
		//	if (!CustomPropertyBuilder.IsEmpty())
		//	{
		//		UClass* CustomPropertyBuilderClass = LoadClass<UPropertyBuilder>(nullptr, *CustomPropertyBuilder);
		//		if (CustomPropertyBuilderClass == nullptr) return;
		//
		//		if (DetailObjectWidgetCreater)
		//		{
		//			DetailObjectWidgetCreater->ConditionalBeginDestroy();
		//			DetailObjectWidgetCreater.Reset();
		//			DetailObjectWidgetCreater = nullptr;
		//		}
		//
		//		DetailObjectWidgetCreater = TStrongObjectPtr<UDetailObjectWidget>(NewObject<UDetailObjectWidget>(GetTransientPackage(), CustomPropertyBuilderClass));
		//	}
		//
		//	const FString CustomFuncRowWidget = Property->GetMetaData(TEXT("FuncPropertyRowWidget"));
		//	if (!CustomFuncRowWidget.IsEmpty())
		//	{
		//
		//	}
		//}

		void SDetailPropertyWidget::SplitterSlotResized(float Size, int32 Index)
	{
		OnSplitterSlotResized.ExecuteIfBound(Index, Size);
		SetSplitterSlotSize(Index, Size);
	}

	void SDetailPropertyWidget::SetSplitterSlotSize(int32 Index, float Size)
	{
		SSplitter::FSlot& Slot = Splitter->SlotAt(Index);
		Slot.SetSizeValue(Size);
	}

	float SDetailPropertyWidget::GetSlotSize() const
	{
		return SplitterSlotSize;
	}

	FReply SDetailPropertyWidget::OnResetClicked()
	{
		return FReply::Handled();
	}

	bool SDetailPropertyWidget::HasSplitter()
	{
		return Splitter.IsValid();
	}
}