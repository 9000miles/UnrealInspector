#include "Node/DetailNode.h"
#include "Core/DetailDefine.h"
#include "Core/DetailFactory.h"
#include "Creater/BoolCreater.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

namespace DETAILS_VIEWER
{


	//FDetailTreeNode::FDetailTreeNode(ENodeType InNodeType, UObject* Object, UE_Property* Property)
	//	: NodeType(InNodeType)
	//{
	//	ObjectPtr = Object;
	//	//PropertyHolder = MakeShareable(new FPropertyHolder(ObjectPtr, Property));

	//}


	//FDetailTreeNode::~FDetailTreeNode()
	//{

	//}

	//FString FDetailTreeNode::GetName()
	//{
	//	//const FText DisplayName = PropertyHolder->GetDisplayName();
	//	//if (DisplayName.IsEmpty())
	//	//{
	//	//	const FString Name = PropertyHolder->GetName();
	//	//	return Name;
	//	//}

	//	//return DisplayName.ToString();
	//	return FString();
	//}

	//TSharedPtr<SWidget> FDetailTreeNode::GetWidget()
	//{
	//	//const FString PropertyType = PropertyHolder->GetPropertyType();
	//	//TSharedPtr<IDetailWidgetCreater> PropertyBuilder = FDetailFactory::Get().FindCreater(PropertyType);
	//	//if (PropertyBuilder == nullptr) return nullptr;

	void FCategoryTreeNode::Copy()
	{
		const FString String = CategoryInfo->Copy();
		FPlatformApplicationMisc::ClipboardCopy(*String);
	}

	void FCategoryTreeNode::Paste()
	{
		FString String;
		FPlatformApplicationMisc::ClipboardPaste(String);
		CategoryInfo->Paste(String);
	}

	bool FCategoryTreeNode::IsCanVisible()
	{
		for (auto& Child : Children)
		{
			if (Child->IsCanVisible()) return true;
		}

		return false;
	}

	//	//return PropertyBuilder->MakeWidget();
	//	return SNullWidget::NullWidget;
	//}

	FString FCategoryTreeNode::GetName()
	{
		return CategoryInfo->Name;
	}


	FString FCategoryTreeNode::GetDisplayName()
	{
		return CategoryInfo->DisplayName;
	}

	TSharedPtr<SWidget> FCategoryTreeNode::GetWidget()
	{
		return /*SNew(SExpandableArea)
			.InitiallyCollapsed(false)
			.HeaderContent()
			[*/
			SNew(STextBlock)
			.Text(FText::FromString(CategoryInfo->Name))
			/*]
			.BodyContent()
			[
			]*/
			;
	}

	void FPropertyTreeNode::Copy()
	{
		const FString String = PropertyInfo->Copy();
		FPlatformApplicationMisc::ClipboardCopy(*String);
	}

	void FPropertyTreeNode::Paste()
	{
		FString String;
		FPlatformApplicationMisc::ClipboardPaste(String);
		PropertyInfo->Paste(String);
	}


	void FPropertyTreeNode::OnSearch(const FText& Text)
	{
		bInSearching = !Text.IsEmpty();

		if (Text.IsEmpty())
		{
			bSearchByName = false;
			bSearchByDisplayName = false;
			return;
		}

		bSearchByName = PropertyInfo->Name.Contains(*Text.ToString(), ESearchCase::IgnoreCase);
		bSearchByDisplayName = PropertyInfo->DisplayName.Contains(*Text.ToString(), ESearchCase::IgnoreCase);
	}

	bool FPropertyTreeNode::IsCanEditable()
	{
		const bool bParentEditable = Parent.IsValid() && Parent->GetIsCanEditable();
		if (!bParentEditable)
		{
			bIsCanEditable = false;
			return false;
		}

		bIsCanEditable = PropertyInfo->Executor->Condition->IsCanEditable();
		return bIsCanEditable;
	}

	bool FPropertyTreeNode::IsCanVisible()
	{
		const bool bParentVisible = Parent.IsValid() && Parent->GetIsCanVisible();
		if (!bParentVisible)
		{
			bIsCanVisible = false;
			return false;
		}

		const bool bCanVisibale = PropertyInfo->Executor->Condition->IsCanVisible();
		const bool bBySearched = bSearchByName || bSearchByDisplayName;
		bIsCanVisible = bInSearching ? bCanVisibale && bBySearched : bCanVisibale;
		return bIsCanVisible;
	}

	FString FPropertyTreeNode::GetName()
	{
		return PropertyInfo->Name;
	}


	FString FPropertyTreeNode::GetDisplayName()
	{
		return PropertyInfo->DisplayName;
	}

	TSharedPtr<SWidget> FPropertyTreeNode::GetWidget()
	{
		////return SNew(SDetailPropertyWidget)
		//if (FWidgetCreaterBool::IsSupport(PropertyInfo))
		//	return MakeWidget<FWidgetCreaterBool>(PropertyHolder);
		////else if (PropertyHolder->IsA(FIntProperty::StaticClass()))
		////	return FWidgetCreaterInt::CreateWidget(PropertyHolder);
		////else if (PropertyHolder->IsA(FInt64Property::StaticClass()))
		////	return FWidgetCreaterInt64::CreateWidget(PropertyHolder);
		////else if (PropertyHolder->IsA(FFloatProperty::StaticClass()))
		////	return FWidgetCreaterFloat::CreateWidget(PropertyHolder);
		////else if (PropertyHolder->IsA(FDoubleProperty::StaticClass()))
		////	return FWidgetCreaterDouble::CreateWidget(PropertyHolder);
		//else if (PropertyHolder->IsA(FStrProperty::StaticClass())
		//	|| PropertyHolder->IsA(FNameProperty::StaticClass()))
		//	return MakeWidget<FWidgetCreaterString>(PropertyHolder);

		return SNullWidget::NullWidget;

	}

	TSharedPtr<SWidget> FTreeNode::GetWidget()
	{
		return SNullWidget::NullWidget;
	}


	void FTreeNode::OnSearch(const FText& Text)
	{
		for (auto& Child : Children)
		{
			Child->OnSearch(Text);
		}
	}

	void FTreeNode::AddChild(TSharedPtr<FTreeNode> Node)
	{
		Children.Add(Node);

		Node->SetParent(AsShared());
	}

}