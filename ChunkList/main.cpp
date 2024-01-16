#include <iostream>
#include <iterator>
#include "ChunkList.h"
#include <list>

using namespace fefu_laboratory_two;
using namespace std;

void print(ChunkList<int, 9> list) {
	for (auto e : list) {
		cout << e << " ";
	}
	cout << "\n";
}

int main()
{
	ChunkList<int, 9> list1;
	ChunkList<int, 9> list2;
	
	for (int i = 0; i < 18; i++) {
		list1.push_back(i);
	}

	for (int i = 0; i < 18; i++) {
		list2.push_back(i);
	}

	print(list1);

	auto const_it = list1.cbegin();
	const_it;
	list1.insert(const_it, 100);
	print(list1);
	list1.pop_front();
	print(list1);/*
	list1.erase(list1.cbegin());
	print(list1);
 	list1.push_back(123);
	list1.push_back(121);
	list1.push_back(121);
	print(list1);*/
	
	
	


	return 0;
}
