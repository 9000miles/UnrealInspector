#include "Creater/TransformCreater.h"
#include "Widgets/Input/SNumericEntryBox.h"

namespace DETAILS_VIEWER
{
#define LOCTEXT_NAMESPACE "SPropertyWidgetTransform"

	void SPropertyWidgetTransform::Construct(const FArguments& InArgs, TSharedPtr<FPropertyInfo> InPropertyInfo)
	{
		SPropertyWidget::Construct(InPropertyInfo);
		const FLinearColor LabelClr = FLinearColor(1.f, 1.f, 1.f, 0.f);

		GetExecutor()->Accessor->Get(Transform);
		const float Width = 80.f;

		ChildSlot[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox).WidthOverride(Width).VAlign(VAlign_Center).HAlign(HAlign_Center)
								[SNew(STextBlock).Text(LOCTEXT("Lable_Translation", "Translation"))]
						]
						+ SHorizontalBox::Slot()
						[
							MakeTranslationWidget(LabelClr)
						]
				]
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox).WidthOverride(Width).VAlign(VAlign_Center).HAlign(HAlign_Center)
								[SNew(STextBlock).Text(LOCTEXT("Lable_Rotation", "Rotation"))]
						]
						+ SHorizontalBox::Slot()
						[
							MakeRotationWidget(LabelClr)
						]
				]
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SBox).WidthOverride(Width).VAlign(VAlign_Center).HAlign(HAlign_Center)
								[SNew(STextBlock).Text(LOCTEXT("Lable_Scale3D", "Scale3D"))]
						]
						+ SHorizontalBox::Slot()
						[
							MakeScale3DWidget(LabelClr)
						]
				]
		];
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Translation_X() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetTranslation().X;
	}


	TSharedRef<SWidget> SPropertyWidgetTransform::MakeTranslationWidget(const FLinearColor LabelClr)
	{
		return SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Traslation_X", "X"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Translation_X)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Translation_X)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Translation_X)
			]
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Traslation_Y", "Y"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Translation_Y)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Translation_Y)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Translation_Y)
			]
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Traslation_Z", "Z"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Translation_Z)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Translation_Z)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Translation_Z)
			];
	}


	TSharedRef<SWidget> SPropertyWidgetTransform::MakeRotationWidget(const FLinearColor LabelClr)
	{
		return SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Rotation_X", "X"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Rotation_X)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Rotation_X)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Rotation_X)
			]
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Rotation_Y", "Y"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Rotation_Y)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Rotation_Y)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Rotation_Y)
			]
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Rotation_Z", "Z"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Rotation_Z)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Rotation_Z)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Rotation_Z)
			]
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Rotation_W", "W"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Rotation_W)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Rotation_W)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Rotation_W)
			];
	}


	TSharedRef<SWidget> SPropertyWidgetTransform::MakeScale3DWidget(const FLinearColor LabelClr)
	{
		return SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Scale3D_X", "X"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Scale3D_X)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Scale3D_X)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Scale3D_X)
			]
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Scale3D_Y", "Y"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Scale3D_Y)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Scale3D_Y)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Scale3D_Y)
			]
			+ SHorizontalBox::Slot()
			.Padding(2).HAlign(HAlign_Fill)
			[
				SNew(SNumericEntryBox<float>)
					.LabelVAlign(VAlign_Center)
					.Label()
					[
						SNew(STextBlock)
							.Text(LOCTEXT("Lable_Scale3D_Z", "Z"))
							.ColorAndOpacity(LabelClr)
					]
					.Value(this, &SPropertyWidgetTransform::GetPropertyValue_Scale3D_Z)
					.AllowSpin(true)
					.OnValueChanged(this, &SPropertyWidgetTransform::OnChanged_Scale3D_Z)
					.OnValueCommitted(this, &SPropertyWidgetTransform::OnCommitted_Scale3D_Z)
			];
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Translation_Y() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetTranslation().Y;
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Translation_Z() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetTranslation().Z;
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Rotation_X() const
	{

		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetRotation().X;
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Rotation_Y() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetRotation().Y;
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Rotation_Z() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetRotation().Z;
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Rotation_W() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetRotation().W;
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Scale3D_X() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetScale3D().X;
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Scale3D_Y() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetScale3D().Y;
	}


	TOptional<float> SPropertyWidgetTransform::GetPropertyValue_Scale3D_Z() const
	{
		FTransform Value;
		GetExecutor()->Accessor->Get(Value);
		return Value.GetScale3D().Z;
	}


	void SPropertyWidgetTransform::OnCommitted_Translation_X(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Translation_X(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Translation_Y(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Translation_Y(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Translation_Z(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Translation_Z(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Rotation_X(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Rotation_X(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Rotation_Y(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Rotation_Y(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Rotation_Z(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Rotation_Z(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Rotation_W(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Rotation_W(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Scale3D_X(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Scale3D_X(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Scale3D_Y(float NewValue, ETextCommit::Type Type)
	{
		OnChanged_Scale3D_Y(NewValue);
	}


	void SPropertyWidgetTransform::OnCommitted_Scale3D_Z(float NewValue, ETextCommit::Type Type)
	{
		Transform.SetScale3D(FVector(Transform.GetScale3D().X, Transform.GetScale3D().Y, NewValue));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Translation_X(float NewValue)
	{
		Transform.SetTranslation(FVector(NewValue, Transform.GetTranslation().Y, Transform.GetTranslation().Z));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Translation_Y(float NewValue)
	{
		Transform.SetTranslation(FVector(Transform.GetTranslation().X, NewValue, Transform.GetTranslation().Z));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Translation_Z(float NewValue)
	{
		Transform.SetTranslation(FVector(Transform.GetTranslation().X, Transform.GetTranslation().Y, NewValue));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Rotation_X(float NewValue)
	{
		Transform.SetRotation(FQuat(NewValue, Transform.GetRotation().Y, Transform.GetRotation().Z, Transform.GetRotation().W));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Rotation_Y(float NewValue)
	{
		Transform.SetRotation(FQuat(Transform.GetRotation().X, NewValue, Transform.GetRotation().Z, Transform.GetRotation().W));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Rotation_Z(float NewValue)
	{
		Transform.SetRotation(FQuat(Transform.GetRotation().X, Transform.GetRotation().Y, NewValue, Transform.GetRotation().W));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Rotation_W(float NewValue)
	{
		Transform.SetRotation(FQuat(Transform.GetRotation().X, Transform.GetRotation().Y, Transform.GetRotation().Z, NewValue));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Scale3D_X(float NewValue)
	{
		Transform.SetScale3D(FVector(NewValue, Transform.GetScale3D().Y, Transform.GetScale3D().Z));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Scale3D_Y(float NewValue)
	{
		Transform.SetScale3D(FVector(Transform.GetScale3D().X, NewValue, Transform.GetScale3D().Z));
		GetExecutor()->Accessor->Set(Transform);
	}


	void SPropertyWidgetTransform::OnChanged_Scale3D_Z(float NewValue)
	{
		Transform.SetScale3D(FVector(Transform.GetScale3D().X, Transform.GetScale3D().Y, NewValue));
		GetExecutor()->Accessor->Set(Transform);
	}
#undef LOCTEXT_NAMESPACE

	TSharedPtr<SWidget> FWidgetCreaterTransform::MakeWidget(TSharedPtr<FTreeNode> TreeNode)
	{
		return MakePropertyWidget<SPropertyWidgetTransform>(TreeNode);
	}

	TArray<FString> FWidgetCreaterTransform::SupportTypes()
	{
		return { TEXT("FTransform"), TEXT("Transform") };
	}

}
