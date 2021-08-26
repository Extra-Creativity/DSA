#include "HuffmanTree.h"
#include<fstream>
const size_t _Size = 1000;
char _Buffer[_Size + 1] = { 0 };
int main()
{
	std::cout << "********Processing*******\n";
	std::ifstream fin("The Old Man And the Sea.txt");
	std::map<char, size_t> Map;
	while (!fin.getline(_Buffer, _Size + 1u).eof())
	{
		size_t len = strlen(_Buffer);
		for (size_t i = 0; i < len; i++)
		{
			if (('a' <= _Buffer[i] && _Buffer[i] <= 'z') || ('A' <= _Buffer[i] && _Buffer[i] <= 'Z')) Map[_Buffer[i]]++;
		}
	}
	fin.close();
	std::cout << "********Processing Over!*********\n\n";

	std::cout << "*******Frequency Table********\n";
	FrequencyTable<char, size_t> m(Map);
	m.print();
	std::cout << "*******Frequency Table Over!********\n\n";

	std::cout << "*******Code Table******\n";
	HuffmanTree<char, size_t> test(m);
	auto Table = test.make_code_table();
	Table.print();
	std::cout << "*******Code Table Over!******\n\n";
	test.level_traverse([](HuffmanTreeNode<char, size_t>* Node) {std::cout << Node->element() << "\\"; });
	return 0;
}
