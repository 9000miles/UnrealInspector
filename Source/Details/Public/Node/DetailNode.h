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

	class FTreeNode :public TSharedFromThis<FTreeNode>, public ITypeName
	{
		IMPLEMENT_ITYPENAME(FTreeNode)
	public:
		FTreeNode(ENodeType	InNodeType) {};
		virtual ~FTreeNode() {}
		virtual FString GetName() { return TEXT("TreeNode"); };
		virtual FString GetDisplayName() { return TEXT("TreeNode"); };
		virtual TSharedPtr<SWidget> GetWidget();

		virtual void Copy() {}
		virtual void Paste() {}

		TArray<TSharedPtr<FTreeNode>> GetChildren() { return Children; }
		TSharedPtr<FTreeNode> GetParent() { return Parent; }
		void AddChild(TSharedPtr<FTreeNode> Node);
		void SetParent(TSharedPtr<FTreeNode> Node) { Parent = Node; }

	protected:
		TSharedPtr<FTreeNode> Parent;
		TArray<TSharedPtr<FTreeNode>> Children;

	protected:
		TSharedPtr<PROPERTY::IExecutor> Executor;
	};

	class FCategoryTreeNode :public FTreeNode
	{
	public:
		FCategoryTreeNode(TSharedPtr<FCategoryInfo> Item) :FTreeNode(ENodeType::Category), CategoryInfo(Item)
		{
		}
		virtual ~FCategoryTreeNode() {}


		void Copy() override;
		void Paste() override;

	public:
		static FString TypeName() { return TEXT("CategoryTreeNode"); }
		FString GetTypeName() override { return FCategoryTreeNode::TypeName(); }
		FString GetName() override;
		FString GetDisplayName() override;
		TSharedPtr<SWidget> GetWidget() override;

	protected:
		TSharedPtr<FCategoryInfo> CategoryInfo;
	};

	class FPropertyTreeNode :public FTreeNode
	{
		IMPLEMENT_ITYPENAME(FPropertyTreeNode)
	public:
		FPropertyTreeNode(TSharedPtr<FPropertyInfo> InPropertyInfo) :FTreeNode(ENodeType::General), PropertyInfo(InPropertyInfo)
		{
			// @TODO ʵ�ָ������Ե������Դ��������飬�ṹ�壬�ȵ�
		}
		virtual ~FPropertyTreeNode() {}

		void Copy() override;
		void Paste() override;

	public:
		FString GetName() override;
		FString GetDisplayName() override;
		TSharedPtr<SWidget> GetWidget() override;

	public:
		TSharedPtr<FPropertyInfo> PropertyInfo;
	};
}