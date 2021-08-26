#include "BiTree.h"
#include<iostream>
#include<vector>
inline void enter() {std::cout << "\n";}

int main()
{
	BiTree<int> tree(1);
	std::cout << tree.height() << " " << tree.size() << "\n";
	auto P = tree.root();
	P->insert_as_left(2);
	P->insert_as_right(3);
	auto curr_node = P->lChild();
	curr_node->insert_as_left(4);
	curr_node->insert_as_right(5);
	std::cout << tree.height() << " " << tree.size() << "\n";
	tree.pre_order_traverse([](decltype(P) Node) {std::cout << Node->data()<<" "; });
	enter();
	tree.in_order_traverse([](decltype(P) Node) {std::cout << Node->data() << " "; });
	enter();
	tree.post_order_traverse([](decltype(P) Node) {std::cout << Node->data() << " "; });
	enter();
	std::cout << P->Curr_tree;
	enter();
	auto K = tree.split_subtree(P);
	K.level_traverse([](decltype(P) Node) {std::cout << Node->data()<< " "; });
	enter();
	auto O = K.root();
	P->swap(O);
	std::cout << tree.root() << " " << O;
	enter();
	P->Curr_tree->level_traverse([](decltype(P) Node) {std::cout << Node->data() << " "; });

	return 0;
}
