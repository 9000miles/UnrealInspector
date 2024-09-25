// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailTest/DetailTestObject.h"
#include "DetailCore/SWidgetHandle.h"
#include "DetailCore/PropertyHolder.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SCompoundWidget.h"

bool UDetailTestObject::IsTickableInEditor() const
{
	return true;
}

void UDetailTestObject::Tick(float DeltaTime)
{
	return;

	const bool bIsDefaultObject = HasAnyFlags(RF_ClassDefaultObject);
	if (bIsDefaultObject) return;

	PrintPropertyValue();

	if (TickTime >= Interval)
	{
		TickTime = 0.f;
		TickChangeProperty();
	}
	else
	{
		TickTime += DeltaTime;
	}
}

TStatId UDetailTestObject::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDetailTestObject, STATGROUP_Tickables);
}

void UDetailTestObject::PrintPropertyValue()
{

	UE_LOG(LogTemp, Log, TEXT("%s"), _Bool_ ? TEXT("TRUE") : TEXT("FALSE"));
	UE_LOG(LogTemp, Log, TEXT("%s"), *_String_);
	UE_LOG(LogTemp, Log, TEXT("%s"), *_Text_.ToString());
	UE_LOG(LogTemp, Log, TEXT("%s"), *_Name_.ToString());

}

void UDetailTestObject::TickChangeProperty()
{
	return;

	_Bool_ = !_Bool_;
	_String_ = FString::Printf(TEXT("String_%d"), FMath::Rand());
	_Text_ = FText::FromString(FString::Printf(TEXT("Text_%d"), FMath::Rand()));
	_Name_ = *FString::Printf(TEXT("Name_%d"), FMath::Rand());
}


FSWidgetHandle UDetailTestObject::CustomPropertyWidget()
{

	return FSWidgetHandle();
}


FSWidgetHandle UDetailTestObject::Property_FuncPropertyRow_RowWidget(FPropertyHandle Property)
{
	class SCustomDetailPropertyRowWidget :public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SCustomDetailPropertyRowWidget)
			{}
		SLATE_END_ARGS()
		void Construct(const FArguments& InArgs)
		{
			ChildSlot
				[
					SNew(SButton).Text(FText::FromString(TEXT("SCustomDetailPropertyRowWidget")))
				];
		}
	};

	TSharedPtr< SCustomDetailPropertyRowWidget> FuncPropertyRow_WidgetPtr1 = SNew(SCustomDetailPropertyRowWidget);
	FuncPropertyRowHandle = MakeShareable(new FSWidgetHandle(FuncPropertyRow_WidgetPtr1));

	return *FuncPropertyRowHandle;
}
//
//FSWidgetHandle UCustomDetailObjectWidget::CreateWidget_Implementation(UObject* Object)
//{
//	class SCustomDetailObjectWidget :public SCompoundWidget
//	{
//	public:
//		SLATE_BEGIN_ARGS(SCustomDetailObjectWidget)
//			{}
//		SLATE_END_ARGS()
//		void Construct(const FArguments& InArgs)
//		{
//			ChildSlot
//				[
//					SNew(SButton).Text(FText::FromString(TEXT("Custom Detail Object Widget")))
//				];
//		}
//	};
//
//	//return FSWidgetHandle();
//	return FSWidgetHandle(SNew(SCustomDetailObjectWidget));
//}
//
//
//TSharedPtr<SPropertyRowWidget> UCustomPropertyBuilder::CreateWidget(TSharedPtr<FPropertyHolder> PropertyHolder)
//{
//	class SCustomPropertyWidget :public SPropertyRowWidget
//	{
//	public:
//		SLATE_BEGIN_ARGS(SCustomPropertyWidget)
//			{}
//		SLATE_END_ARGS()
//
//		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyHolder> InPropertyHolder)
//		{
//			ChildSlot
//				[
//					SNew(SImage)
//				];
//		}
//	};
//
//	return SNew(SCustomPropertyWidget, PropertyHolder);
//}
//
//
//TSharedPtr<SPropertyRowWidget> UCustomDetailRowWidget::CreateWidget(TSharedPtr<FPropertyHolder> PropertyHolder)
//{
//	class SCustomPropertyRowWidget :public SPropertyRowWidget
//	{
//	public:
//		SLATE_BEGIN_ARGS(SCustomPropertyRowWidget)
//			{}
//		SLATE_END_ARGS()
//
//		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyHolder> InPropertyHolder)
//		{
//			ChildSlot
//				[
//					SNew(SHorizontalBox)
//						+ SHorizontalBox::Slot()
//						[
//							SNew(SImage)
//						]
//						+ SHorizontalBox::Slot()
//						[
//							SNew(STextBlock).Text(FText::FromString(TEXT("Custom Property Row Widget")))
//						]
//				];
//		}
//	};
//
//	return SNew(SCustomPropertyRowWidget, PropertyHolder);
//}
//
