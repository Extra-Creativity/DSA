#pragma once
#include "BiTreeBase.h"

template<typename _Ty>
class BiTree;

template<typename _Ty>
struct BiTreeNode: public BiTreeNodeBase<BiTree<_Ty>,BiTreeNode<_Ty>,_Ty>
{
	friend class BiTreeBase<BiTree<_Ty>,BiTreeNode<_Ty>, _Ty>;
	friend class BiTree<_Ty>;
	friend struct BiTreeNode<_Ty>;
	friend struct BiTreeNodeBase<BiTree<_Ty>, BiTreeNode<_Ty>, _Ty>;
	using _Nodeptr = BiTreeNode<_Ty>*;
	using _Super = BiTreeNodeBase<BiTree<_Ty>,BiTreeNode<_Ty>, _Ty>;
	using _Treeptr = BiTree<_Ty>*;
	~BiTreeNode() {
	}
protected:
	BiTreeNode(_Nodeptr Parent, _Nodeptr lChild, _Nodeptr rChild, _Treeptr _Curr_tree, const _Ty& _Ele) :\
		_Super(Parent, lChild, rChild, _Curr_tree, _Ele) {
		this->_Set_self(this);
	};
};

template<typename _Ty>
class BiTree :public BiTreeBase<BiTree<_Ty>,BiTreeNode<_Ty>,_Ty>
{
	friend struct BiTreeNodeBase<BiTree<_Ty>, BiTreeNode<_Ty>, _Ty>;
	friend struct BiTreeNode<_Ty>;
	using _Nodeptr = BiTreeNode<_Ty>*;
	using _Super = BiTreeBase<BiTree<_Ty>,BiTreeNode<_Ty>, _Ty>;
public:
	BiTree(const _Ty& _Ele, _Nodeptr Parent = nullptr, _Nodeptr lChild = nullptr, _Nodeptr rChild = nullptr) :\
		_Super(_Ele, Parent, lChild, rChild) {
		this->_Set_self(this);
	};
	BiTree(BiTree& another_tree) :_Super(another_tree) {
		this->_Set_all_self(this);
	};
	BiTree(BiTree&& another_tree) noexcept :_Super(std::move(another_tree)){
		this->_Set_all_self(this);
	};
protected:
	BiTree():_Super() {};
};
