// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SDetailPropertyWidget.h"
#include "SlateOptMacros.h"
#include "View/SPropertyName.h"
#include "Creater/DetailCreater.h"

namespace DETAILS_VIEWER
{
	static float SplitterSlotSize = 0.f;

	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
		void SDetailPropertyWidget::Construct(const FArguments& InArgs, TSharedPtr<FTreeNode> Node, bool bOverrideRowWidget, TSharedPtr<SWidget> InWidget)
	{
		SDetailTreeItem::Construct(Node);

		OnSplitterSlotResized = InArgs._OnSplitterSlotResized;



		SplitterSlotSize = 0.3f;


		TSharedPtr<SBox> Box = SNew(SBox)
			;

		TSharedPtr<SWidget> CustomDetailRowWidget = InWidget;
		if (CustomDetailRowWidget.IsValid() && bOverrideRowWidget)
		{
			Box->SetContent(CustomDetailRowWidget.ToSharedRef());
		}
		else
		{
			//const bool bOverrideRowWidget = DetailNodePtr->OverrideRowWidget();

			TSharedPtr<SWidget> PropertyWidget = InWidget.IsValid() ? InWidget : GetPropertyNode()->PropertyInfo->Executor->WidgetMaker->MakeWidget();
			TSharedPtr<SPropertyName> PropertyName = SNew(SPropertyName, Node);

			TSharedPtr<SWidget> Widget = PropertyWidget;
			if (!PropertyWidget)
			{
				FText Text = FText::FromString(FString::Printf(TEXT("%s Not Implemented"), *GetPropertyNode()->PropertyInfo->Type));
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
		//	TSharedPtr<FPropertyHolder> Proxy = DetailNodePtr->GetPropertyHolder();
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