#include <iostream>
#include "FibbonacciHeap.hxx"


int main()
{
	FibbonacciHeap<WorldMap> heap({ { "Italy","Rome" }, { "Spain","Madrid" }, { "Ukraine","Kyiv" }, { "Spain","Barcelona" }, { "Russia","Moscow" }, { "Italy","Neapol" } });
	FibbonacciHeap<WorldMap> heap2({ { "Germany","Nurnburg" }, { "Sweden","Stokholm" }, { "USA","Washington DC" }, { "France","Paris" } });

	FibbonacciHeap<WorldMap> heap3 = *heap.fibHeapUnion(&heap2);

	std::cout << heap3.getWebGraphviz("g");

	std::cout << "\n";

	std::cout << heap3.fibHeapExtractMin()->key << std::endl;

	std::cout << heap3.getWebGraphviz("g") << std::endl;

	heap3.fibHeapDecreaseKey({ "Spain","Madrid" }, { "Spain", "Barcelona"});

	std::cout << heap3.getWebGraphviz("Decreased Madrid to Barcelona") << std::endl;

	heap3.fibHeapDelete({ "Sweden","Stokholm" });

	std::cout << heap3.getWebGraphviz("g");

	return 0;
}