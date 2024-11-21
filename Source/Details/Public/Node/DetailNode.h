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
		virtual int32 GetDepth() { return Depth; }

		virtual void Copy() {}
		virtual void Paste() {}
		virtual void OnSearch(const FText& Text);
		virtual bool IsCanEditable() { return true; }
		virtual bool IsCanVisible() { return true; }

		TArray<TSharedPtr<FTreeNode>> GetChildren() { return Children; }
		TSharedPtr<FTreeNode> GetParent() { return Parent; }
		void AddChild(TSharedPtr<FTreeNode> Node);
		void SetParent(TSharedPtr<FTreeNode> Node) { Parent = Node; Depth = Node->GetDepth() + 1; }

		bool GetIsCanVisible() { return bIsCanVisible; }
		bool GetIsCanEditable() { return bIsCanEditable; }

	protected:
		int32 Depth = 0;
		bool bIsCanVisible = true;
		bool bIsCanEditable = true;
		TSharedPtr<FTreeNode> Parent;
		TArray<TSharedPtr<FTreeNode>> Children;

	protected:
		TSharedPtr<PROPERTY::IExecutor> Executor;
	};

	class FCategoryTreeNode :public FTreeNode
	{
		IMPLEMENT_ITYPENAME(FCategoryTreeNode)
	public:
		FCategoryTreeNode(TSharedPtr<FCategoryInfo> Item) :FTreeNode(ENodeType::Category), CategoryInfo(Item)
		{
		}
		virtual ~FCategoryTreeNode() {}


		void Copy() override;
		void Paste() override;
		bool IsCanVisible() override;


	public:
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
		void OnSearch(const FText& Text) override;

		bool IsCanEditable() override;
		bool IsCanVisible() override;

	public:
		FString GetName() override;
		FString GetDisplayName() override;
		TSharedPtr<SWidget> GetWidget() override;

	public:
		TSharedPtr<FPropertyInfo> PropertyInfo;

		bool bInSearching = false;
		bool bSearchByName;
		bool bSearchByDisplayName;
	};
}