// Fill out your copyright notice in the Description page of Project Settings.


#include "View/SDetailProperty.h"
#include "SlateOptMacros.h"
#include "View/SPropertyName.h"
#include "Creater/DetailCreater.h"

namespace DETAILS_VIEWER
{
	static float SplitterSlotSize = 0.f;

	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
		void SDetailProperty::Construct(const FArguments& InArgs, TSharedPtr<FTreeNode> InTreeNode, bool bOverrideRowWidget, TSharedPtr<SWidget> InWidget)
	{
		//SDetailTreeItem::Construct(StaticCastSharedPtr<FPropertyTreeNode>(InTreeNode));
		SDetailTreeItem::Construct(InTreeNode);

		OnSplitterSlotResized = InArgs._OnSplitterSlotResized;



		//SplitterSlotSize = 0.3f - (NodePtr->GetDepth() * 0.005f);
		SplitterSlotSize = 0.3f;


		TSharedPtr<SBox> Box = SNew(SBox)
			.HAlign(HAlign_Fill)
			;

		TSharedPtr<SWidget> CustomDetailRowWidget = InWidget;
		if (CustomDetailRowWidget.IsValid() && bOverrideRowWidget)
		{
			Box->SetContent(CustomDetailRowWidget.ToSharedRef());
		}
		else
		{
			//const bool bOverrideRowWidget = DetailNodePtr->OverrideRowWidget();

			TSharedPtr<SWidget> PropertyWidget = InWidget.IsValid() ? InWidget : GetNode<FPropertyTreeNode>()->PropertyInfo->Executor->WidgetMaker->MakeWidget(NodePtr);
			TSharedPtr<SPropertyName> PropertyName = SNew(SPropertyName, InTreeNode);

			TSharedPtr<SWidget> Widget = PropertyWidget;
			if (!PropertyWidget)
			{
				FText Text = FText::FromString(FString::Printf(TEXT("%s Not Implemented"), *GetNode<FPropertyTreeNode>()->PropertyInfo->Type));
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
					.OnSlotResized(this, &SDetailProperty::SplitterSlotResized, 0)
					[
						PropertyName.ToSharedRef()
					]
					+ SSplitter::Slot()
					//.Value(SplitterSlotSize)
					.SizeRule(SSplitter::ESizeRule::FractionOfParent)
					.OnSlotResized(this, &SDetailProperty::SplitterSlotResized, 1)
					[
						SNew(SBorder)
							.BorderImage(FAppStyle::Get().GetBrush("NoBorder"))
							.Padding(FMargin(5, 0, 3, 0))
							[
								SNew(SHorizontalBox)
									.IsEnabled(this, &SDetailProperty::IsCanEditable)
									+ SHorizontalBox::Slot()
									.HAlign(HAlign_Fill)
									.VAlign(VAlign_Center)
									[
										Widget.ToSharedRef()
									]
									+ SHorizontalBox::Slot()
									.Padding(3, 0)
									.VAlign(VAlign_Center)
									.AutoWidth()
									[
										SNew(SButton)
											.OnClicked(this, &SDetailProperty::OnResetClicked)
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

		void SDetailProperty::SplitterSlotResized(float Size, int32 Index)
	{
		OnSplitterSlotResized.ExecuteIfBound(Index, Size);
		SetSplitterSlotSize(Index, Size);
	}

	void SDetailProperty::SetSplitterSlotSize(int32 Index, float Size)
	{
		//Size = Size + (NodePtr->GetDepth() * 0.005f);
		SSplitter::FSlot& Slot = Splitter->SlotAt(Index);
		Slot.SetSizeValue(Size);
	}

	float SDetailProperty::GetSlotSize() const
	{
		return SplitterSlotSize;//- (NodePtr->GetDepth() * 0.1f);
	}

	FReply SDetailProperty::OnResetClicked()
	{
		GetNode<FPropertyTreeNode>()->PropertyInfo->Executor->Accessor->Reset();
		return FReply::Handled();
	}

	bool SDetailProperty::HasSplitter()
	{
		return Splitter.IsValid();
	}

	float SDetailProperty::GetSplitterSlotSize()
	{
		return SplitterSlotSize - (NodePtr->GetDepth() * 0.1f);
	}

	bool SDetailProperty::IsCanEditable() const
	{
		TSharedPtr< FPropertyTreeNode> Node = GetNode<FPropertyTreeNode>();
		return Node->PropertyInfo->Executor->Condition->CanEdit();
	}

}