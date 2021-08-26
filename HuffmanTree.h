#pragma once
#include "BiTreeBase.h"
#include<set>
#include<queue>
#include<map>
#include<iostream>

template<typename _Ty,typename _Times_Ty>
struct FrequencyPair
{
	_Ty _Element;
	_Times_Ty _Times;
	FrequencyPair(const _Ty& Ele, const _Times_Ty Times) :_Element(Ele), _Times(Times) {};
	bool operator<(const FrequencyPair& another)const { return _Times < another._Times; }
};

template<typename _Ty, typename _Times_Ty>
std::ostream& operator<<(std::ostream& os, FrequencyPair<_Ty, _Times_Ty>& _Pair)
{
	os << _Pair._Element << " " << _Pair._Times;
	return os;
}

template<typename _Ty, typename _Times_Ty>
struct FrequencyTable:public std::vector<FrequencyPair<_Ty,_Times_Ty>>
{
	template<typename _It1,typename _It2>
	FrequencyTable(_It1 _Begin, _It1 _End, _It2 _Times_begin, _It2 _Times_end)
	{
		while (_Begin != _End && _Times_begin != _Times_end)
		{
			this->emplace_back(*_Begin, *_Times_begin);
			_Begin++, _Times_begin++;
		}
	}

	FrequencyTable(std::map<_Ty, _Times_Ty> _Map){
		for (auto& _Pair : _Map)
		{
			this->emplace_back(_Pair.first, _Pair.second);
		}
	}

	void print(std::ostream& os = std::cout){
		for (auto i = this->begin(); i != this->end(); i++){
			os << *i << "\n";
		}
		return;
	}
};

template<typename _Ty>
struct CodePair{
	_Ty _Element;
	std::vector<bool> _Code;
	CodePair(const _Ty& Ele, const std::vector<bool>& Code) :_Element(Ele), _Code(Code) {};
};

template<typename _Ty>
std::ostream& operator<<(std::ostream& os, CodePair<_Ty>& _Pair)
{
	os << _Pair._Element << " ";
	for (auto i : _Pair._Code){
		os << i;
	}
	return os;
}

template<typename _Ty>
struct CodeTable:public std::vector<CodePair<_Ty>>{
	void print(std::ostream& os = std::cout) {
		for (auto i = this->begin(); i != this->end(); i++) {
			os << *i << "\n";
		}
		return;
	}
};

template<typename _Ty = char, typename _Times_Ty = size_t>
class HuffmanTree;

template<typename _Ty = char, typename _Times_Ty = size_t>
struct HuffmanTreeNode :public BiTreeNodeBase<HuffmanTree<_Ty,_Times_Ty>,HuffmanTreeNode<_Ty,_Times_Ty>,_Times_Ty>
{
	using _Nodeptr = HuffmanTreeNode<_Ty,_Times_Ty>*;
	using _Super = BiTreeNodeBase<HuffmanTree<_Ty, _Times_Ty>,HuffmanTreeNode<_Ty,_Times_Ty>, _Times_Ty>;
	using _Tree_Ty = HuffmanTree<_Ty, _Times_Ty>;
	using _Treeptr = _Tree_Ty*;
	friend class _Tree_Ty;
private:
	_Ty _Element;
public:
	_Times_Ty times() { return this->_Data; }
	_Ty element() { return _Element; }
	HuffmanTreeNode(_Nodeptr Parent, _Nodeptr lChild, _Nodeptr rChild, _Treeptr _Curr_tree,const _Ty& _Ele,const _Times_Ty& _Times):\
		_Super(Parent, lChild, rChild, _Curr_tree, _Times),_Element(_Ele){
		this->_Set_self(this);
	};
	struct HuffmanNodePtrCmp {
		bool operator()(const _Nodeptr& _Node1, const _Nodeptr& _Node2)const {
			return _Node2->_Data < _Node1->_Data;
		}
	};
};

template<typename _Ty,typename _Times_Ty>
class HuffmanTree :public BiTreeBase<HuffmanTree<_Ty,_Times_Ty>,HuffmanTreeNode<_Ty,_Times_Ty>,_Times_Ty>
{
	using _Node_Ty = HuffmanTreeNode<_Ty, _Times_Ty>;
	using _Nodeptr = _Node_Ty*;
	using _Super = BiTreeBase<HuffmanTree<_Ty, _Times_Ty>, HuffmanTreeNode<_Ty, _Times_Ty>, _Times_Ty>;
	friend struct _Node_Ty;
public:
	HuffmanTree(const FrequencyTable<_Ty, _Times_Ty>& Table) :_Super(),_Table(Table)
	{
		using _Pair = FrequencyPair<_Ty, _Times_Ty>;
		std::priority_queue<_Nodeptr,std::vector<_Nodeptr>,_Node_Ty::HuffmanNodePtrCmp> Q;
		_Nodeptr _New_node = nullptr;
		for (auto& i : Table)
		{
			_New_node = new _Node_Ty(nullptr, nullptr, nullptr, this, i._Element, i._Times);
			Q.push(_New_node);
		}
		while (Q.size() > 1u)
		{
			_Nodeptr _New_left_node = Q.top();
			Q.pop();
			_Nodeptr _New_right_node = Q.top();
			Q.pop();
			_New_node = new _Node_Ty(nullptr, _New_left_node, _New_right_node, this, _Ty(), _New_left_node->_Data + _New_right_node->_Data);
			_New_left_node->_Ptr._Parent = _New_right_node->_Ptr._Parent = _New_node;
			Q.push(_New_node);
		}
		this->_Root = this->_Curr_node = _New_node;
	};

	CodeTable<_Ty> make_code_table(){
		CodeTable<_Ty> _Store_table;
		_Extend(this->_Root, _Store_table, std::vector<bool>());
		return _Store_table;
	}

private:
	const FrequencyTable<_Ty, _Times_Ty>& _Table;

	static void _Extend(_Nodeptr _Node,CodeTable<_Ty>& _Store_table,std::vector<bool> _Curr_arr){
		if (!_Node) return;
		//If the left child exists, the right one also exists.
		if (_Node->has_lChild()){
			_Curr_arr.push_back(false);
			_Extend(_Node->_Ptr._lChild, _Store_table, _Curr_arr);
			_Curr_arr.pop_back();

			_Curr_arr.push_back(true);
			_Extend(_Node->_Ptr._rChild, _Store_table, _Curr_arr);
		}
		else {
			_Store_table.emplace_back(_Node->_Element, _Curr_arr);
		}
		return;
	}
};
