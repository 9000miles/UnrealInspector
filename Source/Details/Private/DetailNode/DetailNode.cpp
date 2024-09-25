#include "DetailNode/DetailNode.h"
#include "DetailCore/DetailDefine.h"
#include "DetailCore/DetailFactory.h"

namespace DetailsViewer
{


	FDetailTreeNode::FDetailTreeNode(EDetailNodeType InNodeType, UObject* Object, UE_Property* Property)
		: NodeType(InNodeType)
	{
		ObjectPtr = Object;
		PropertyHolder = MakeShareable(new FPropertyHolder(ObjectPtr, Property));
	}


	FDetailTreeNode::~FDetailTreeNode()
	{

	}

	FString FDetailTreeNode::GetName()
	{
		const FText DisplayName = PropertyHolder->GetDisplayName();
		if (DisplayName.IsEmpty())
		{
			const FString Name = PropertyHolder->GetName();
			return Name;
		}

		return DisplayName.ToString();
	}

	TSharedPtr<SWidget> FDetailTreeNode::GetWidget()
	{
		const FString PropertyType = PropertyHolder->GetPropertyType();
		TSharedPtr<IDetailWidgetCreater> PropertyBuilder = FDetailFactory::Get().FindCreater(PropertyType);
		if (PropertyBuilder == nullptr) return nullptr;

		return PropertyBuilder->MakeWidget();
	}
}