#include <iostream>
#include "BPlusTree.h"

int main() {
	

	/*std::string input;
	while (1) {
		system("cls");
		std::cout << tree.getWebGraphviz() << std::endl;
		std::cout << "Delete (\"nope\" for exit) : ";
		std::cin >> input;
		if (input != "nope") {
			tree.remove(std::atoi(input.c_str()));
			continue;
		}
		break;
	}*/

	BPlusTree<WorldMap, 2> tree({ { "Italy","Rome" }, { "Spain","Madrid" }, { "Ukraine","Kyiv" }, { "Spain","Barcelona" }, { "Russia","Moscow" }, { "Italy","Neapol" } });

	std::cout << tree.getWebGraphviz();

	system("cls");
	std::cout << tree.getWebGraphviz() << std::endl;
}