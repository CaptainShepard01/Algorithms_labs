#include <iostream>
#include "RBTree.h"

int main()
{
	auto tree = new RBTree<WorldMap>({ { "Italy","Rome" }, { "Spain","Madrid" }, { "Ukraine","Kyiv" }, { "Spain","Barcelona" }, { "Russia","Moscow" }, { "Italy","Neapol" } }, true);

	std::cout << "Initial state:\n\n";
	std::cout << tree->getWebGraphvizPersistent() << "\n\n";

	tree->skipBack();
	std::cout << "After skipBack():\n\n";
	std::cout << tree->getWebGraphvizPersistent() << "\n\n";

	tree->insertPersistent({ "Ukraine","Varash" });
	std::cout << "Add Varash, Ukraine:\n\n";
	std::cout << tree->getWebGraphvizPersistent() << "\n\n";

	tree->insertPersistent({ "Russia", "Magadan" });
	std::cout << "Add Magadan, Russia:\n\n";
	std::cout << tree->getWebGraphvizPersistent() << "\n\n";

	tree->skipBack();
	std::cout << "After skipBack(():\n\n";
	std::cout << tree->getWebGraphvizPersistent() << "\n\n";

	std::cout << "Remove Kyiv, Ukraine:\n\n";
	tree->removePersistent({ "Ukraine","Kyiv" });
	std::cout << tree->getWebGraphvizPersistent() << "\n\n";

	tree->skipBack();
	std::cout << "After skipBack(():\n\n";
	std::cout << tree->getWebGraphvizPersistent() << "\n\n";

	return 0;
}