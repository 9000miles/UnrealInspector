// Fill out your copyright notice in the Description page of Project Settings.
#include "Detail/UObjectDetail.h"
namespace DETAILS_VIEWER
{
	namespace PARAMETER
	{

		bool FUEPropertyEditable::CanEdit()
		{
			return true;
		}

		bool FUEPropertyVisible::CanVisible()
		{
			return true;
		}

		FString FUEPropertyDefaultGetter::GetDefault()
		{
			return TEXT("");
		}

		TSharedRef<SWidget> FUEPropertyWidgetMaker::MakeWidget(const FString& Value)
		{
			return SNullWidget::NullWidget;
		}

		void FUEPropertyCopyExecutor::Execute()
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

		void FUEPropertyPasteExecutor::Execute()
		{
			throw std::logic_error("The method or operation is not implemented.");
		}
	}

	FUObjectDetail::FUObjectDetail()
	{

	}

	void FUObjectDetail::SetObject(TWeakObjectPtr<UObject> InObject)
	{
		Object = InObject;

		if (!Object.IsValid())
			return;

		DetailInfo = MakeShareable(new FDetailInfo());
		DetailInfo->Name = Object->GetName();
		DetailInfo->DisplayName = Object->GetName();
		DetailInfo->Description = Object->GetName();
		DetailInfo->DetailExecutor = MakeShareable(new FUObjectDetailExecutor());

		IteratorField(Object, DetailInfo->CategoryList);
	}

	void FUObjectDetail::IteratorField(TWeakObjectPtr<UObject> InObject, TSharedPtr<FCategoryList> CategoryList)
	{
		check(InObject.IsValid());

		UClass* Class = InObject->GetClass();
		for (TFieldIterator<UE_Property> PropertyIt(Class, EFieldIteratorFlags::IncludeSuper); PropertyIt; ++PropertyIt)
		{
			auto Property = *PropertyIt;
			const FString PropertyName = Property->GetName();
			const FString PropertyType = Property->GetCPPType();
			const FString Category = Property->GetMetaData(TEXT("Category"));
			const FString DisplayName = Property->GetMetaData(TEXT("DisplayName"));

			TSharedPtr<ICategoryInfo> ExistCategory = CategoryList->Find(Category);
			if (!ExistCategory.IsValid())
			{
				TSharedPtr<ICategoryInfo> CategoryInfo = MakeShareable(new ICategoryInfo());
				ExistCategory = CategoryInfo;
				CategoryList->Add(CategoryInfo);
			}

			TSharedPtr<IParameterInfo> Parameter = MakeShareable(new IParameterInfo());

			Parameter->Name = PropertyName;
			Parameter->DisplayName = DisplayName;
			Parameter->Description = Property->GetMetaData(TEXT("Description"));
			Parameter->Type = Property->GetCPPType();
			Parameter->Category = Category;
			Parameter->Advanced = Property->HasMetaData(TEXT("AdvancedDisplay"));
			Parameter->Executor = MakeShareable(new PARAMETER::FUObjectParameterExecutor());
			Parameter->Metadata = MakeShareable(new PARAMETER::FMetadata());

			ExistCategory->Add(Parameter);
		}
	}
}