// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DetialManger.h"
#include "Core/DetailDefine.h"
#include "Detail/UObjectDetail.h"
#include "DetailTestObject.h"
#include "DetailsViewerGameInstance.generated.h"

/**
 * $Comment$
 */
UCLASS(BlueprintType, Blueprintable)
class UDetailsViewerGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	void StartGameInstance() override
	{
		Super::StartGameInstance();

		FDetialManager& Manager = FDetialManager::Get();

		TSharedPtr<FDetailOptions> Options = MakeShared<FDetailOptions>();
		TSharedPtr<FUObjectDetailHolder> DetailHolder = Manager.Create<FUObjectDetailHolder>(Options);

		static TStrongObjectPtr<UDetailTestObject> TestObject = TStrongObjectPtr<UDetailTestObject>(NewObject<UDetailTestObject>());
		DetailHolder->SetObject(TestObject.Get());

		//TSharedPtr<SWindow> WindowPtr = SNew(SWindow)
		//	.ClientSize(FVector2D(800, 600))
		//	.HasCloseButton(true)
		//	.SupportsMaximize(true)
		//	;
		TSharedPtr<SWidget> ViewWidget = DetailHolder->GetWidget();
		GetGameViewportClient()->AddViewportWidgetContent(ViewWidget.ToSharedRef());
		//WindowPtr->SetContent(ViewWidget.ToSharedRef());

		SetInput();
	}

	void SetInput()
	{
		GetPrimaryPlayerController()->bShowMouseCursor = true;
	}

};