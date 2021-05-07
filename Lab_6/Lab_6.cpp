#include <iostream>
#include "BPTree.h"

int main() {
	BPTree<WorldMap, 2> tree({ { "Italy","Rome" }, { "Spain","Madrid" }, { "Ukraine","Kyiv" }, { "Spain","Barcelona" }, { "Russia","Moscow" }, { "Italy","Neapol" } });

	std::cout << "Initial state:\n\n";
	std::cout << tree.getWebGraphviz() << std::endl;

	tree.deleteData({ "Spain", "Madrid" });

	std::cout << "Deleted Spain, Madrid:\n\n";
	std::cout << tree.getWebGraphviz() << std::endl;

	tree.insert({ "Ukraine", "Kyiv" });

	std::cout << "Inserted Ukraine, Kyiv:\n\n";
	std::cout << tree.getWebGraphviz() << std::endl;

	tree.insert({ "France", "Paris" });

	std::cout << "Inserted France, Paris:\n\n";
	std::cout << tree.getWebGraphviz() << std::endl;

	tree.deleteData({ "Ukraine", "Kyiv" });

	std::cout << "Deleted Ukraine, Kyiv:\n\n";		
	std::cout << tree.getWebGraphviz() << std::endl;

	tree.insert({ "Ukraine", "Obukhiv" });
	tree.insert({ "Ukraine", "Rivne" });
	tree.insert({ "Ukraine", "Dnipro" });
	tree.insert({ "Ukraine", "Lviv" });

	std::cout << "Deleted Ukraine, Kyiv:\n\n";
	std::cout << tree.getWebGraphviz() << std::endl;

	return 0;
}