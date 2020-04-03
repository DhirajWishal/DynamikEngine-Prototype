#include "wbafx.h"
using namespace Dynamik;
#include <vector>
#include <list>

#include "DataTypesLib/Public/HashMap.h"

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

int main() {
    std::string str = "";

    while (true)
    {
        std::cout << "Enter String: ";
        std::getline(std::cin, str);
        std::cout << "Generated Hash: "; 
        getHash(str.c_str());
        std::cout << "\n\n";
    }

	return 0;
}