// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "Core/DetailObjectWidget.h"
#include "Core/SWidgetHandle.h"
#include "Core/DetailDefine.h"
#include "Core/DetialManger.h"
#include "Detail/UObjectDetail.h"
#include "Core/DetailInfo.h"
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
	UPROPERTY(EditAnywhere, Category = "AAA", meta = (DisplayName = "窝窝屎"))
	bool Bool_1 = true;
	UPROPERTY(EditAnywhere, meta = (HintText = "This is FString Property"))
	FString String_1 = TEXT("sjoifwoeo");
	UPROPERTY(EditAnywhere)
	FText Text_1;
	UPROPERTY()
	FName Name_1;
	UPROPERTY(meta = (UIMin = 0, UIMax = 100))
	int32 int32_Min_Max;


	UPROPERTY(meta = (UIMin = 0.55, UIMax = 50.1))
	float float_Min_Max;

	UPROPERTY()
	FVector2D Vector2D_1 = FVector2D(34, 534);
	UPROPERTY()
	FVector Vector_1 = FVector(34, 534, 744);
	UPROPERTY()
	FVector4 Vector4_1 = FVector4(34, 534, 67, 100);

	UPROPERTY()
	FGuid Guid_1;

	UPROPERTY(meta = (PropertyBuilder = "/Script/RuntimeDetail.CustomPropertyBuilder"))
	int32 _PropertyBuilder = 456;
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


namespace DETAILS_VIEWER_TEST
{
	class FDetailsViewerTestModule
	{
	public:
		static void RunTest()
		{
			TestJsonMetadata();

			FDetialManager& Manager = FDetialManager::Get();

			TSharedPtr<FDetailOptions> Options = MakeShared<FDetailOptions>();
			TSharedPtr<FUObjectDetailHolder> DetailHolder = Manager.Create<FUObjectDetailHolder>(Options);

			static TStrongObjectPtr<UDetailTestObject> TestObject = TStrongObjectPtr<UDetailTestObject>(NewObject<UDetailTestObject>());
			DetailHolder->SetObject(TestObject.Get());

			TSharedPtr<SWindow> WindowPtr = SNew(SWindow)
				.ClientSize(FVector2D(800, 600))
				.HasCloseButton(true)
				.SupportsMaximize(true)
				;
			TSharedPtr<SWidget> ViewWidget = DetailHolder->GetWidget();
			WindowPtr->SetContent(ViewWidget.ToSharedRef());

			FSlateApplication::Get().AddWindow(WindowPtr.ToSharedRef());
		}

		static void TestJsonMetadata()
		{
			const FString File = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("Inspector/Source/Details/detail-info.json"));
			TSharedPtr<FJsonObject> JsonObject = LoadJsonFile(File);
			TSharedPtr<DETAILS_VIEWER::PROPERTY::FMetadata> Metadata = MakeShared<DETAILS_VIEWER::PROPERTY::FMetadata>();
			Metadata->FromJson(JsonObject);
			FString ff = Metadata->Get<FString>(FString(TEXT("name")));
		}

		static TSharedPtr<FJsonObject> LoadJsonFile(const FString& InFileName)
		{
			TSharedPtr<FJsonObject> JsonObject;

			FString JsonContents;
			if (FFileHelper::LoadFileToString(JsonContents, *InFileName))
			{
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContents);
				FJsonSerializer::Deserialize(Reader, JsonObject);
			}

			return JsonObject;
		}

		static void RunTest1()
		{

			//static FDetialManager* DetManager = FDetialManager::Get();

			//FDetailOptions Options;
			//UDetailViewer* DetViewer = DetManager->CreateDetail(Options);

			//static TStrongObjectPtr<UDetailTestObject> TestObject = TStrongObjectPtr<UDetailTestObject>(NewObject<UDetailTestObject>(DetViewer));
			//DetViewer->SetObject(TestObject.Get());

			//TSharedPtr<SWindow> WindowPtr = SNew(SWindow)
			//	.ClientSize(FVector2D(800, 600))
			//	.HasCloseButton(true)
			//	.SupportsMaximize(true)
			//	;
			//TSharedPtr<SWidget> ViewWidget = DetViewer->GetViewWidget();
			//WindowPtr->SetContent(ViewWidget.ToSharedRef());

			//FSlateApplication::Get().AddWindow(WindowPtr.ToSharedRef());
		}
	};

}