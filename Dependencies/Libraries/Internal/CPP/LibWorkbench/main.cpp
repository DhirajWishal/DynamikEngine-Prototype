#include "wbafx.h"
using namespace Dynamik;
#include <vector>
#include <list>
/*
 Main entrypoint for the C++ library Workbench.

 Author:	Wishal Dhiraj
 Data:		15/03/2020
 Language:  C/C++
 Project:   Workbench
 IDE:       Microsoft Visual Studio Community 2019
*/

/*
 Idea: a dynamic array consists of addresses.
 data is allocated and the address is stored in the address store.
 When retrieving it referes the store and return the dereferenced address.s
 PROS: indexed allocation (not contigeious)
*/

#define SET_TYPE_NAME(...) #__VA_ARGS__
#define TYPE_TO_STRING(...) #__VA_ARGS__

struct TypeData {
	VPTR data = nullptr;

	FSTR myTypeName;
};

template<class TYPE>
TypeData func(TYPE value)
{
	TypeData myType;
	myType.myTypeName = TYPE_TO_STRING(TYPE);
	return myType;
}

struct Types {
	VPTR myAddress;
	FSTR myTypeName;

	template<class TYPE>
	void setTypeName(TYPE)
	{
		myTypeName = typeid(TYPE).name();
	}
};

int main() {
	SimpleNode<UI32> myNode;
	SimpleNode<UI32> myNode2;
	myNode.myData = 100;
	myNode2.myData = 69;

	SimpleLinkedList<UI32> list;
	//list.push(myNode);
	//list.push(myNode2);
	//list.push(myNode);

	SimpleNode<UI32> node;
	for (UI32 i = 0; i < 128000; i++) {
		node.myData = i;
		list.push(node);
	}

	std::vector<UI32> nums;

	UI32 myNum;
	for (UI32 i = 0; i < 128000; i++) {
		myNum = i;
		nums.push_back(myNum);
	}

	ARRAY<UI32> nums2;

	UI32 myNum2;
	for (UI32 i = 0; i < 128000; i++) {
		myNum2 = i;
		nums2.push_back(myNum2);
	}

	SimpleNode<UI32> myNode1 = list.get(1);
	SimpleNode<UI32> myNode3 = list.pop(1);

	return 0;
}