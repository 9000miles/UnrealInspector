// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/DetailTestObject.h"
#include "Core/SWidgetHandle.h"
#include "Core/PropertyHolder.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SCompoundWidget.h"

bool UDetailTestObject::IsTickableInEditor() const
{
	return true;
}

void UDetailTestObject::Tick(float DeltaTime)
{
	//return;

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
//return;
#define PRINT_VALUE(Name, Format, Value) \
    UE_LOG(LogTemp, Log, TEXT(#Name ": " Format), Value);

	UClass* ObjectClass = StaticClass();
	for (TFieldIterator<FProperty> It(ObjectClass); It; ++It)
	{
		FProperty* Property = *It;
		//if (Property->HasAnyPropertyFlags(CPF_BlueprintVisible))
		{
			FString PropertyName = Property->GetNameCPP();
			FString JsonValue = PROPERTY::FUEPropertyHelper::PropertyToJson(this, *PropertyName);
			UE_LOG(LogTemp, Log, TEXT("%s: %s"), *PropertyName, *JsonValue);
		}
	}
}

void UDetailTestObject::TickChangeProperty()
{
	return;

	Bool_1 = !Bool_1;
	String_1 = FString::Printf(TEXT("String_%d"), FMath::Rand());
	//Text_1 = FText::FromString(FString::Printf(TEXT("Text_%d"), FMath::Rand()));
	Name_1 = *FString::Printf(TEXT("Name_%d"), FMath::Rand());
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
