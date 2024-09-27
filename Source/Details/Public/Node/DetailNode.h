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

	class FTreeNode :public ITypeName
	{
	public:
		virtual FString GetName() { return TEXT("TreeNode"); };
		virtual TSharedPtr<SWidget> GetWidget();

		TArray<TSharedPtr<FTreeNode>> GetChildren() { return Children; }
		TSharedPtr<FTreeNode> GetParent() { return Parent; }
		void AddChild(TSharedPtr<FTreeNode> Node);

	protected:
		TSharedPtr<FTreeNode> Parent;
		TArray<TSharedPtr<FTreeNode>> Children;
	public:
		FString GetTypeName() override;

	};

	class FCategoryTreeNode :public FTreeNode
	{
	public:
		FCategoryTreeNode(TSharedPtr<FCategoryInfo> Item) :CategoryInfo(Item)
		{
		}

	public:
		static FString TypeName() { return TEXT("CategoryTreeNode"); }
		FString GetTypeName() override { return FCategoryTreeNode::TypeName(); }

		TSharedPtr<FCategoryInfo> CategoryInfo;

		FString GetName() override;


		TSharedPtr<SWidget> GetWidget() override;
	};

	class FPropertyTreeNode :public FTreeNode
	{
	public:
		FPropertyTreeNode(TSharedPtr<FPropertyInfo> Item) :PropertyInfo(Item)
		{
		}

	public:
		static FString TypeName() { return TEXT("PropertyTreeNode"); }
		FString GetTypeName() override { return FPropertyTreeNode::TypeName(); }

		TSharedPtr<FPropertyInfo> PropertyInfo;

		FString GetName() override;


		TSharedPtr<SWidget> GetWidget() override;

	};
}