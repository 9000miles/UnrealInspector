#include "Node/DetailNode.h"
#include "Core/DetailDefine.h"
#include "Core/DetailFactory.h"
#include "Creater/BoolCreater.h"

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

	void FTreeNode::AddChild(TSharedPtr<FTreeNode> Node)
	{
		Children.Add(Node);

		Node->SetParent(AsShared());
	}

}