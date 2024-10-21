#pragma once

#include "CoreMinimal.h"
#include "../Core/DetailDefine.h"
#include "Core/DetailFactory.h"
#include "Core/PropertyHolder.h"


namespace DETAILS_VIEWER
{

	//class FDetailTreeNode :public TSharedFromThis<FDetailTreeNode>
	//{
	//public:
	//	FDetailTreeNode(ENodeType InNodeType, UObject* Object, UE_Property* Property);
	//	virtual ~FDetailTreeNode();

	//	virtual FString GetName();
	//	virtual TSharedPtr<SWidget> GetWidget();

	//	TArray<TSharedPtr<FDetailTreeNode>> GetChildNodes() { return ChildNodes; }
	//	TSharedPtr<FDetailTreeNode> GetParentNode() { return ParentNode; }

	//	//TSharedPtr<FPropertyHolder> GetPropertyHolder() { return PropertyHolder; }
	//	TWeakObjectPtr<UObject> GetObject() { return ObjectPtr; }
	//	ENodeType GetNodeType() { return NodeType; }

	//protected:
	//	ENodeType NodeType;
	//	TWeakObjectPtr<UObject> ObjectPtr;
	//	TSharedPtr<FDetailTreeNode> ParentNode;
	//	TArray<TSharedPtr<FDetailTreeNode>> ChildNodes;
	//};

	class FTreeNode :public ITypeName
	{
	public:
		FTreeNode(ENodeType	InNodeType) {};
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

	protected:
		TSharedPtr<PROPERTY::IExecutor> Executor;
	};

	class FCategoryTreeNode :public FTreeNode
	{
	public:
		FCategoryTreeNode(TSharedPtr<FCategoryInfo> Item) :FTreeNode(ENodeType::General), CategoryInfo(Item)
		{
		}

	public:
		static FString TypeName() { return TEXT("CategoryTreeNode"); }
		FString GetTypeName() override { return FCategoryTreeNode::TypeName(); }
		FString GetName() override;
		TSharedPtr<SWidget> GetWidget() override;

	protected:
		TSharedPtr<FCategoryInfo> CategoryInfo;
	};

	class FPropertyTreeNode :public FTreeNode
	{
	public:
		FPropertyTreeNode(TSharedPtr<FPropertyInfo> InPropertyInfo) :FTreeNode(ENodeType::General), PropertyInfo(InPropertyInfo)
		{
		}

	public:
		static FString TypeName() { return TEXT("PropertyTreeNode"); }
		FString GetTypeName() override { return FPropertyTreeNode::TypeName(); }
		FString GetName() override;
		TSharedPtr<SWidget> GetWidget() override;

	public:
		TSharedPtr<FPropertyInfo> PropertyInfo;
	};
}