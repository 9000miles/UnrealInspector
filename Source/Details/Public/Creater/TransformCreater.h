// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetailCreater.h"
#include "Core/PropertyHolder.h"
#include "Core/DetailInfo.h"

namespace DETAILS_VIEWER
{
	/**
	 *
	 */
	class DETAILSVIEWER_API SPropertyWidgetTransform :public SPropertyWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyWidgetTransform)
			{}
		SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo);

		TSharedRef<SWidget> MakeNameWidget(FText Text);

		TSharedRef<SWidget> MakeTranslationWidget(const FLinearColor LabelClr);
		TSharedRef<SWidget> MakeRotationWidget(const FLinearColor LabelClr);
		TSharedRef<SWidget> MakeScale3DWidget(const FLinearColor LabelClr);

		TOptional<float> GetPropertyValue_Translation_X() const;
		TOptional<float> GetPropertyValue_Translation_Y() const;
		TOptional<float> GetPropertyValue_Translation_Z() const;
		TOptional<float> GetPropertyValue_Rotation_X() const;
		TOptional<float> GetPropertyValue_Rotation_Y() const;
		TOptional<float> GetPropertyValue_Rotation_Z() const;
		TOptional<float> GetPropertyValue_Rotation_W() const;
		TOptional<float> GetPropertyValue_Scale3D_X() const;
		TOptional<float> GetPropertyValue_Scale3D_Y() const;
		TOptional<float> GetPropertyValue_Scale3D_Z() const;

		void OnCommitted_Translation_X(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Translation_Y(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Translation_Z(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Rotation_X(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Rotation_Y(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Rotation_Z(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Rotation_W(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Scale3D_X(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Scale3D_Y(float NewValue, ETextCommit::Type Type);
		void OnCommitted_Scale3D_Z(float NewValue, ETextCommit::Type Type);

		void OnChanged_Translation_X(float NewValue);
		void OnChanged_Translation_Y(float NewValue);
		void OnChanged_Translation_Z(float NewValue);
		void OnChanged_Rotation_X(float NewValue);
		void OnChanged_Rotation_Y(float NewValue);
		void OnChanged_Rotation_Z(float NewValue);
		void OnChanged_Rotation_W(float NewValue);
		void OnChanged_Scale3D_X(float NewValue);
		void OnChanged_Scale3D_Y(float NewValue);
		void OnChanged_Scale3D_Z(float NewValue);

	private:
		FTransform Transform;
	};
	/**
	 *
	 */
	class DETAILSVIEWER_API FWidgetCreaterTransform : public FPropertyWidgetCreater
	{
		IMPLEMENT_ITYPENAME(FWidgetCreaterTransform)
	public:
		TSharedPtr<SWidget> MakeWidget(TSharedPtr<FTreeNode> TreeNode) override;
		TArray<FString> SupportTypes() override;
	};

};