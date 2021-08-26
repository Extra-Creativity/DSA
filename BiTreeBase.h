#pragma once
#include<xutility>
#include<stack>

template<typename _Tree_Ty,typename _Node_Ty,typename _Ty>
class BiTreeBase;

template<typename _Tree_Ty,typename _Node_Ty,typename _Ty>
struct BiTreeNodeBase
{
	friend class BiTreeBase<_Tree_Ty,_Node_Ty, _Ty>;
	using _Nodeptr = _Node_Ty*;
	using _Treeptr = _Tree_Ty*;
private:
	struct NodePtrCombination
	{
		_Nodeptr _Parent, _lChild, _rChild;
		_Nodeptr _Selfptr;
		NodePtrCombination(_Nodeptr Parent, _Nodeptr lChild, _Nodeptr rChild) :\
			_Parent(Parent), _lChild(lChild), _rChild(rChild), _Selfptr(nullptr) 
		{};
	};
public:
	_Treeptr Curr_tree;
	~BiTreeNodeBase(){	};

	inline bool has_lChild()const { return static_cast<bool>(_Ptr._lChild); }
	inline bool has_rChild()const { return static_cast<bool>(_Ptr._rChild); }
	inline const _Nodeptr& parent() const { return _Ptr._Parent; }
	inline const _Nodeptr& lChild() const { return _Ptr._lChild; }
	inline const _Nodeptr& rChild() const { return _Ptr._rChild; }
	inline const _Nodeptr& sibling() const {
		_Nodeptr& _Parent = _Ptr._Parent;
		if (_Parent) {
			if (_Ptr._Selfptr == _Parent->_Ptr._lChild) return _Parent->_Ptr._rChild;
			else return _Parent->_Ptr._lChild;
		}
		return nullptr;
	}
	inline _Ty& data() { return _Data; }
	inline size_t subtree_size()const { return _Size; }
	inline int height()const { return _Height; };

	_Nodeptr insert_as_left(_Nodeptr Node)
	{
		_Nodeptr& _lChild = _Ptr._lChild;
		if (_lChild == Node) return Node;
		if (Node)
		{
			if (Node == _Ptr._Selfptr) {
				_Nodeptr _Newnode = _Copy_subtree(Node, Curr_tree);
				if (_lChild) _lChild->~_Node_Ty();
				_Newnode->_Ptr._Parent = Node;
				Node->_Ptr._lChild = _Newnode;
				return _Newnode;
			}

			auto& _Parent = Node->_Ptr._Parent;
			if (_Parent)
			{
				if (_Parent->_Ptr._lChild == Node) _Parent->_Ptr._lChild = nullptr;
				else _Parent->_Ptr._rChild = nullptr;
				_Parent->Curr_tree->_Update_height_above(_Parent);
				_Parent->Curr_tree->_Update_size_above(_Parent);
			}
			Node->_Ptr._Parent = _Ptr._Selfptr;
		}
		if (_lChild) _lChild->~_Node_Ty();
		_lChild = Node;
		Curr_tree->_Update_height_above(_Ptr._Selfptr);
		Curr_tree->_Update_size_above(_Ptr._Selfptr);
		return Node;
	}

	_Nodeptr insert_as_right(_Nodeptr Node)
	{
		_Nodeptr& _rChild = _Ptr._rChild;
		if (_rChild == Node) return Node;
		if (Node)
		{
			if (Node == _Ptr._Selfptr) {
				_Nodeptr _Newnode = _Copy_subtree(Node, Curr_tree);
				if (_rChild) _rChild->~_Node_Ty();
				_Newnode->_Ptr._Parent = Node;
				Node->_Ptr._rChild = _Newnode;
				return _Newnode;
			}

			auto& _Parent = Node->_Ptr._Parent;
			if (_Parent)
			{
				if (_Parent->_Ptr._lChild == Node) _Parent->_Ptr._lChild = nullptr;
				else _Parent->_Ptr._rChild = nullptr;
				_Parent->Curr_tree->_Update_height_above(_Parent);
				_Parent->Curr_tree->_Update_size_above(_Parent);
			}
			Node->_Ptr._Parent = _Ptr._Selfptr;
		}
		if (_rChild) _rChild->~_Node_Ty();
		_rChild = Node;
		Curr_tree->_Update_height_above(_Ptr._Selfptr);
		Curr_tree->_Update_size_above(_Ptr._Selfptr);
		return Node;
	}

	_Nodeptr insert_as_left(const _Ty& _Ele)
	{
		_Nodeptr& _lChild = _Ptr._lChild;
		_Nodeptr Node = new _Node_Ty(_Ptr._Selfptr, nullptr, nullptr, Curr_tree, _Ele);
		if (_lChild) _lChild->~_Node_Ty();
		_lChild = Node;
		Curr_tree->_Update_height_above(_Ptr._Selfptr);
		Curr_tree->_Update_size_above(_Ptr._Selfptr);
		return Node;
	}

	_Nodeptr insert_as_right(const _Ty& _Ele)
	{
		_Nodeptr& _rChild = _Ptr._rChild;
		_Nodeptr Node = new _Node_Ty(_Ptr._Selfptr, nullptr, nullptr, Curr_tree, _Ele);
		if (_rChild) _rChild->~_Node_Ty();
		_rChild = Node;
		Curr_tree->_Update_height_above(_Ptr._Selfptr);
		Curr_tree->_Update_size_above(_Ptr._Selfptr);
		return Node;
	}

	void swap(_Nodeptr Node) {
		if (Node == _Ptr._Selfptr) return;
		auto _My_parent = _Ptr._Parent;
		auto _Node_parent = Node->_Ptr._Parent;
		if (_My_parent)
		{
			if (_My_parent->_Ptr._lChild == _Ptr._Selfptr) {
				_My_parent->_Ptr._lChild = Node;
			}
			else {
				_My_parent->_Ptr._rChild = Node;
			}
		}else {
			Curr_tree->_Root = Curr_tree->_Curr_node = Node;
		}
		if (_Node_parent)
		{
			if (_Node_parent->_Ptr._lChild == Node->_Ptr._Selfptr) {
				_Node_parent->_Ptr._lChild = Node;
			}
			else {
				_Node_parent->_Ptr._rChild = Node;
			}
		} else {
			Node->Curr_tree->_Root = Node->Curr_tree->_Curr_node = _Ptr._Selfptr;
		}
		_Ptr._Parent = _Node_parent;
		Node->_Ptr._Parent = _My_parent;
		std::swap(Curr_tree, Node->Curr_tree);
		Node->Curr_tree->_Set_all_self(Node->Curr_tree), Curr_tree->_Set_all_self(Curr_tree);
		Curr_tree->_Update_height_above(_My_parent), Curr_tree->_Update_size_above(_My_parent);
		Node->Curr_tree->_Update_height_above(_Node_parent), Node->Curr_tree->_Update_size_above(_Node_parent);
		return;
	}

protected:
	NodePtrCombination _Ptr;
	_Ty _Data;
	int _Height;
	size_t _Size;

	BiTreeNodeBase(_Nodeptr Parent, _Nodeptr lChild, _Nodeptr rChild, _Treeptr _Curr_tree, const _Ty& _Ele) :\
		_Ptr(Parent, lChild, rChild), Curr_tree(_Curr_tree), _Data(_Ele)
	{
		if (lChild)
		{
			if (rChild)
			{
				_Height = std::max(lChild->_Height, rChild->_Height) + 1;
				_Size = lChild->_Size + rChild->_Size + 1;
				lChild->Curr_tree = rChild->Curr_tree = Curr_tree;
			}
			else
			{
				_Height = lChild->_Height + 1;
				_Size = lChild->_Size + 1;
				lChild->Curr_tree = Curr_tree;
			}
		}
		else if (rChild)
		{
			_Height = rChild->_Height + 1;
			_Size = rChild->_Size + 1;
			rChild->Curr_tree = Curr_tree;
		}
		else _Height = 0, _Size = 1u;
	};

	//If you want to copy the subtree, please use split_subtree.To avoid error, copying a single node is prohibited.
	BiTreeNodeBase(BiTreeNodeBase&) = delete;
	BiTreeNodeBase(const BiTreeNodeBase&) = delete;
	BiTreeNodeBase(BiTreeNodeBase&&) = delete;

	static int _Calc_height(const _Nodeptr _Node){ return _Node ? _Node->_Height : -1; };

	static size_t _Calc_size(const _Nodeptr _Node){ return _Node ? _Node->_Size : 0; }

	virtual int _Update_height() {
		return _Height = std::max(_Calc_height(_Ptr._lChild),_Calc_height(_Ptr._rChild)) + 1;
	}

	size_t _Update_size(){
		return _Size = _Calc_size(_Ptr._lChild) + _Calc_size(_Ptr._rChild) + 1;
	}

	static _Nodeptr _Copy_subtree(_Nodeptr _Pos, _Treeptr _Curr_tree)
	{
		_Nodeptr _Node = new _Node_Ty(nullptr, nullptr, nullptr, _Curr_tree, _Pos->_Data);
		auto& _Curr_lChild = _Node->_Ptr._lChild;
		auto& _Curr_rChild = _Node->_Ptr._rChild;
		if (_Pos->has_lChild()) {
			_Curr_lChild = _Copy_subtree(_Pos->_Ptr._lChild, _Curr_tree);
			_Curr_lChild ->_Ptr._Parent = _Node;
		}
		if (_Pos->has_rChild()) {
			_Curr_rChild = _Copy_subtree(_Pos->_Ptr._rChild, _Curr_tree);
			_Curr_rChild->_Ptr._Parent = _Node;
		}
		_Node->_Ptr._Selfptr = _Node;
		return _Node;
	}

	static void _Goto_deepest(std::stack<_Nodeptr>& S)
	{
		_Nodeptr _Pos = S.top();
		while (true)
		{
			if (_Pos->has_lChild())
			{
				if (_Pos->has_rChild()) S.push(_Pos->_Ptr._rChild);
				S.push(_Pos = _Pos->_Ptr._lChild);
			}
			else if (_Pos->has_rChild())
			{
				S.push(_Pos = _Pos->_Ptr._rChild);
			}
			else break;
		}
		return;
	}

	template<typename _Pr>
	static void _Visit_along_left(std::stack<_Nodeptr>& S, _Pr& _Pred)
	{
		_Nodeptr _Pos = S.top();
		S.pop();
		while (_Pos)
		{
			_Pred(_Pos);
			S.push(_Pos->_Ptr._rChild);
			_Pos = _Pos->_Ptr._lChild;
		}
		return;
	}

	void _Goto_leftmost(std::stack<_Nodeptr>& S) const {
		_Nodeptr _Pos = _Ptr._Selfptr;
		while (_Pos)
		{
			S.push(_Pos);
			_Pos = _Pos->_Ptr._lChild;
		}
		return;
	}

	static void _Destroy(_Nodeptr _Node) {
		auto& _Parent = _Node->_Ptr._Parent;
		if (_Parent) {
			if (_Node == _Parent->_Ptr._lChild)
				_Parent->_Ptr._lChild = nullptr;
			else _Parent->_Ptr._rChild = nullptr;
		} 
		delete _Node; 
	};

	void _Set_self(_Nodeptr _Self) { _Ptr._Selfptr = _Self; return; }

};

template<typename _Tree_Ty,typename _Node_Ty, typename _Ty>
class BiTreeBase
{
	friend struct BiTreeNodeBase<BiTreeBase<_Tree_Ty,_Node_Ty, _Ty>,_Node_Ty, _Ty>;
	using _Nodeptr = _Node_Ty*;
protected:
	_Nodeptr _Curr_node;
	_Nodeptr _Root;

	virtual void _Update_height_above(_Nodeptr _Pos)
	{
		while (_Pos)
		{
			int _Old_height = _Pos->_Height;
			if (_Old_height == _Pos->_Update_height()) break;
			_Pos = _Pos->_Ptr._Parent;
		}
		return;
	}

	void _Update_size_above(_Nodeptr _Pos)
	{
		while (_Pos)
		{
			_Pos->_Update_size();
			_Pos = _Pos->_Ptr._Parent;
		}
		return;
	}

	void _Set_self(_Tree_Ty* _Ptr) { _Self =_Curr_node-> Curr_tree = _Ptr; }

	void _Set_all_self(_Tree_Ty* _Ptr) { pre_order_traverse([&](_Nodeptr _Node) {_Node->Curr_tree = _Ptr; }); }

	BiTreeBase() :_Curr_node(nullptr), _Self(nullptr), _Root(nullptr) {};

public:
	inline _Nodeptr root() const{ return _Root; }
	inline size_t size() const { return _Curr_node->_Size; }
	inline int height() const { return _Curr_node->_Height; };

	template<typename _Pr>
	void post_order_traverse(const _Pr& _Pred, _Nodeptr _Pos)
	{
		if (!_Pos) return;
		std::stack<_Nodeptr> S;
		S.push(_Pos);
		_Nodeptr _Parent = _Pos->parent();
		while (!S.empty())
		{
			if (S.top() != _Parent)
				_Node_Ty::_Goto_deepest(S);
			_Pos = S.top();
			S.pop();
			_Parent = _Pos->parent();
			_Pred(_Pos);
		}
		return;
	}

	template<typename _Pr>
	void post_order_traverse(const _Pr& _Pred) {
		post_order_traverse(_Pred, _Curr_node);
		return;
	}

	template<typename _Pr>
	void pre_order_traverse(const _Pr& _Pred, _Nodeptr _Pos) {
		std::stack<_Nodeptr> S;
		S.push(_Pos);
		while (!S.empty()) {
			BiTreeNodeBase<_Tree_Ty,_Node_Ty,_Ty>::_Visit_along_left(S, _Pred);
		}
		return;
	}

	template<typename _Pr>
	void pre_order_traverse(const _Pr& _Pred) {
		pre_order_traverse(_Pred, _Curr_node);
		return;
	}

	template<typename _Pr>
	void in_order_traverse(const _Pr& _Pred, _Nodeptr _Pos) {
		if (!_Pos) return;
		std::stack<_Nodeptr> S;
		while (true)
		{
			_Pos->_Goto_leftmost(S);
			do
			{
				if (S.empty()) goto End;
				_Pos = S.top();
				S.pop();
				_Pred(_Pos);
			} while(!(_Pos = _Pos->_Ptr._rChild));
		}
	End:
		return;
	}

	template<typename _Pr>
	void in_order_traverse(const _Pr& _Pred) {
		in_order_traverse(_Pred, _Curr_node);
		return;
	}

	template<typename _Pr>
	void level_traverse(const _Pr& _Pred, _Nodeptr _Pos){
		std::deque<_Nodeptr> Q{_Pos};
		while (!Q.empty())
		{
			_Pos = Q.front();
			Q.pop_front();
			if (_Pos->has_lChild()) Q.push_back(_Pos->_Ptr._lChild);
			if (_Pos->has_rChild()) Q.push_back(_Pos->_Ptr._rChild);
			_Pred(_Pos);
		}
		return;
	}

	template<typename _Pr>
	void level_traverse(const _Pr& _Pred) {
		level_traverse(_Pred, _Curr_node);
		return;
	}

	void erase(_Nodeptr _Pos) {
		if (_Pos) {
			auto _Parent = _Pos->_Ptr._Parent;
			_Node_Ty::_Destroy(_Pos);
			if (_Parent){
				_Parent->Curr_tree->_Update_height_above(_Parent);
				_Parent->Curr_tree->_Update_size_above(_Parent);
			}
		} return;
	}

	BiTreeBase(const _Ty& _Ele,const _Nodeptr Parent = nullptr,const _Nodeptr lChild = nullptr,const _Nodeptr rChild = nullptr) :\
		_Curr_node(new _Node_Ty(Parent, lChild, rChild, _Self, _Ele)),_Self(nullptr)
	{
		_Root = Parent ? Parent->Curr_tree->_Root : _Curr_node;
	};

	BiTreeBase(BiTreeBase& another_tree)
	{
		_Curr_node = BiTreeNodeBase<_Tree_Ty, _Node_Ty, _Ty>::_Copy_subtree(another_tree._Root, nullptr);
	};

	BiTreeBase(BiTreeBase&& another_tree) noexcept{
		_Root = another_tree._Root, _Curr_node = another_tree._Curr_node;
		_Self = another_tree._Self;
		another_tree._Root = another_tree._Curr_node = nullptr;
	}

	~BiTreeBase() { 
		if (_Curr_node)
		{
			auto _Parent = _Curr_node->_Ptr._Parent;
			post_order_traverse(_Node_Ty::_Destroy);
			if (_Parent)
			{
				_Parent->Curr_tree->_Update_height_above(_Parent);
				_Parent->Curr_tree->_Update_size_above(_Parent);
			}
		}
	}

	_Tree_Ty split_subtree(const _Nodeptr _Node) const
	{
		_Tree_Ty _Subtree(_Node->_Data);
		_Nodeptr _Temp = _Node_Ty::_Copy_subtree(_Node, &_Subtree);
		_Subtree._Curr_node = _Subtree._Root = _Temp;
		return _Subtree;
	};
private:
	_Tree_Ty* _Self;
};
