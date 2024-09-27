#pragma once

#include "CoreMinimal.h"
#include "../Core/DetailDefine.h"
#include "Core/DetailFactory.h"
#include "Core/PropertyHolder.h"


namespace DETAILS_VIEWER
{

	class FDetailTreeNode :public TSharedFromThis<FDetailTreeNode>
	{
	public:
		FDetailTreeNode(EDetailNodeType InNodeType, UObject* Object, UE_Property* Property);
		virtual ~FDetailTreeNode();

		virtual FString GetName();
		virtual TSharedPtr<SWidget> GetWidget();

		TArray<TSharedPtr<FDetailTreeNode>> GetChildNodes() { return ChildNodes; }
		TSharedPtr<FDetailTreeNode> GetParentNode() { return ParentNode; }

		TSharedPtr<FPropertyHolder> GetPropertyHolder() { return PropertyHolder; }
		TWeakObjectPtr<UObject> GetObject() { return ObjectPtr; }
		EDetailNodeType GetNodeType() { return NodeType; }

	protected:
		EDetailNodeType NodeType;
		TWeakObjectPtr<UObject> ObjectPtr;
		TSharedPtr<FPropertyHolder> PropertyHolder;
		TSharedPtr<FDetailTreeNode> ParentNode;
		TArray<TSharedPtr<FDetailTreeNode>> ChildNodes;
	};
}