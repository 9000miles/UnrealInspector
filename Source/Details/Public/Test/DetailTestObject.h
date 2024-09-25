// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "Core/DetailObjectWidget.h"
#include "Core/SWidgetHandle.h"
#include "Core/DetailDefine.h"
#include "Core/DetialManger.h"
#include "DetailTestObject.generated.h"

/**
 *
 */
UCLASS()
class DETAILSVIEWER_API UDetailTestObject : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	bool IsTickableInEditor() const override;
	void Tick(float DeltaTime) override;
	TStatId GetStatId() const override;

	void PrintPropertyValue();
private:
	void TickChangeProperty();

public:
	UFUNCTION()
	FSWidgetHandle CustomPropertyWidget();
	UFUNCTION()
	FSWidgetHandle Property_FuncPropertyRow_RowWidget(FPropertyHandle Property);

public:
	UPROPERTY(meta = (DisplayName = "窝窝屎"))
	bool _Bool_;
	UPROPERTY(EditAnywhere, meta = (HintText = "This is FString Property"))
	FString _String_;
	UPROPERTY(EditAnywhere)
	FText _Text_;
	UPROPERTY()
	FName _Name_;
	//UPROPERTY(meta = (UIMin = 0, UIMax = 100))
	//int32 _int32_;


	UPROPERTY(meta = (PropertyBuilder = "/Script/RuntimeDetail.CustomPropertyBuilder"))
	int32 _PropertyBuilder;
	UPROPERTY(meta = (PropertyBuilder = "/Script/RuntimeDetail.CustomDetailRowWidget"))
	int32 _DetailRowWidget;
	UPROPERTY(meta = (FuncPropertyRowWidget = "Property_FuncPropertyRow_RowWidget"))
	int32 FuncPropertyRow;
	TSharedPtr<FSWidgetHandle> FuncPropertyRowHandle;


private:
	int32 Interval = 3;
	float TickTime = 0.f;
};
//
//UCLASS()
//class UCustomPropertyBuilder :public FPropertyRowWidgetCreater
//{
//	GENERATED_BODY()
//public:
//	TSharedPtr<SPropertyRowWidget> CreateWidget(TSharedPtr<FPropertyHolder> PropertyHolder) override;
//};
//
//UCLASS()
//class DETAILSVIEWER_API UCustomDetailRowWidget :public FPropertyRowWidgetCreater
//{
//	GENERATED_BODY()
//public:
//	TSharedPtr<SPropertyRowWidget> CreateWidget(TSharedPtr<FPropertyHolder> PropertyHolder) override;
//	virtual bool OverrideRowWidget() { return true; }
//
//};

//
//UCLASS(meta = (CustomDetail = "/Script/RuntimeDetail.CustomDetailObjectWidget"))
//class DETAILSVIEWER_API UCustomDetailObject : public UObject
//{
//	GENERATED_BODY()
//};
//
//UCLASS()
//class DETAILSVIEWER_API UCustomDetailObjectWidget : public UDetailObjectWidget
//{
//	GENERATED_BODY()
//public:
//	FSWidgetHandle CreateWidget_Implementation(UObject* Object) override;
//};


namespace DetailsViewerTest
{
	class FDetailsViewerTestModule
	{
	public:
		static void RunTest()
		{

			static UDetialManager* DetManager = UDetialManager::Get();

			FDetailOptions Options;
			UDetailViewer* DetViewer = DetManager->CreateDetail(Options);

			static TStrongObjectPtr<UDetailTestObject> TestObject = TStrongObjectPtr<UDetailTestObject>(NewObject<UDetailTestObject>(DetViewer));
			DetViewer->SetObject(TestObject.Get());

			TSharedPtr<SWindow> WindowPtr = SNew(SWindow)
				.ClientSize(FVector2D(800, 600))
				.HasCloseButton(true)
				.SupportsMaximize(true)
				;
			TSharedPtr<SWidget> ViewWidget = DetViewer->GetViewWidget();
			WindowPtr->SetContent(ViewWidget.ToSharedRef());

			FSlateApplication::Get().AddWindow(WindowPtr.ToSharedRef());
		}
	};

}