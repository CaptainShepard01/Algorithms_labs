#include "BPlusTree.h"

template<typename T, size_t degree>
BPlusTree<T, degree>::BPlusTree() {
	root = new BPlusTreeNode<T>();
	root->isLeaf = true;
}

template<typename T, size_t degree>
BPlusTree<T, degree>::BPlusTree(const std::initializer_list<T>& list) {
	root = new BPlusTreeNode<T>();
	root->isLeaf = true;

	for (const auto& item : list) {
		insert(item);
	}
}


template<typename T, size_t degree>
std::pair<BPlusTreeNode<T>*, size_t> BPlusTree<T, degree>::search(BPlusTreeNode<T>* current, const T& key) {
	while (!current->isLeaf) {
		size_t keysSize = current->keys.size();

		if (key >= current->keys[keysSize - 1]) {
			current = current->children[keysSize];
			continue;
		}

		for (size_t i = 0; i < keysSize; i++) {
			if (key < current->keys[i]) {
				current = current->children[i];
				break;
			}
			if (key == current->keys[i]) {
				current = current->children[i + 1];
				break;
			}
		}
	}

	for (size_t i = 0, n = current->keys.size(); i < n; i++) {
		if (current->keys[i] == key)
			return { current, i };
	}
	return { nullptr, -1 };
}

template<typename T, size_t degree>
std::pair<BPlusTreeNode<T>*, size_t> BPlusTree<T, degree>::search(const T& key) {
	return search(root, key);
}

template<typename T, size_t degree>
void BPlusTree<T, degree>::splitChild(BPlusTreeNode<T>* parent, size_t childIndex) {
	BPlusTreeNode<T>* newNode = new BPlusTreeNode<T>;
	BPlusTreeNode<T>* toSplit = parent->children[childIndex];

	newNode->isLeaf = toSplit->isLeaf;

	for (size_t j = 0; j < degree; j++) {
		newNode->keys.push_back(toSplit->keys[j + degree]);
	}

	if (!toSplit->isLeaf) {
		for (size_t j = 0; j < degree; j++) {
			newNode->children.push_back(toSplit->children[j + degree + 1]);
		}
		toSplit->children.resize(degree + 1);
	}

	parent->children.insert(parent->children.begin() + (childIndex + 1), newNode);

	parent->keys.insert(parent->keys.begin() + childIndex, newNode->keys[0]);

	if (!newNode->isLeaf)
		newNode->keys.erase(newNode->keys.begin());

	toSplit->keys.resize(degree);

	auto prevToSplitSibling = toSplit->sibling;

	if (newNode->isLeaf) {
		toSplit->sibling = newNode;
		if (prevToSplitSibling)
			newNode->sibling = prevToSplitSibling;
	}
}

template<typename T, size_t degree>
void BPlusTree<T, degree>::insertNonfull(BPlusTreeNode<T>* current, const T& key) {
	size_t keysSize = current->keys.size();

	if (keysSize == 0) {
		current->keys.push_back(key);
		return;
	}

	for (size_t i = 0; i < keysSize; i++) {
		if (current->keys[i] >= key) {
			current->keys.insert(current->keys.begin() + i, key);
			return;
		}
	}

	current->keys.insert(current->keys.begin() + (keysSize), key);
}

template<typename T, size_t degree>
void BPlusTree<T, degree>::insert(const T& key) {
	BPlusTreeNode<T>* current = root;

	while (!current->isLeaf) {
		size_t keysSize = current->keys.size();

		if (current->keys[keysSize - 1] < key) {
			current = current->children[keysSize];
			continue;
		}

		for (size_t i = 0; i < keysSize; i++) {
			if (current->keys[i] >= key) {
				current = current->children[i];
				break;
			}
		}
	}

	insertNonfull(current, key);

	if (current->keys.size() == 2 * degree) {
		recursiveSplit(current);
	}
}

template<typename T, size_t degree>
void BPlusTree<T, degree>::recursiveSplit(BPlusTreeNode<T>* current) {
	if (current->keys.size() != 2 * degree)
		return;

	if (current == root) {
		BPlusTreeNode<T>* newRoot = new BPlusTreeNode<T>();
		newRoot->children.push_back(current);

		root = newRoot;
		splitChild(newRoot, 0);

		return;
	}
	auto [parent, index] = findParent(root, current);

	splitChild(parent, index);

	recursiveSplit(parent);
}

template<typename T, size_t degree>
void BPlusTree<T, degree>::remove(const T& key) {
	auto [removeTarget, removeTargetIndex] = search(key);
	auto [targetParent, childIndex] = findParent(root, removeTarget);

	if (removeTarget == nullptr)
		return;

	removeTarget->keys.erase(removeTarget->keys.begin() + removeTargetIndex);

	if (root->keys.empty())
		return;

	if (childIndex > 0 && removeTargetIndex == 0 && removeTarget->keys.size() >= 1) {
		targetParent->keys[childIndex - 1] = removeTarget->keys[0];
	}

	if (removeTarget->keys.size() == degree - 2) {
		auto leftSibling = findLeftSibling(removeTarget),
			rightSibling = findRightSibling(removeTarget);

		bool hasRightSibling = rightSibling != nullptr,
			hasLeftSibling = leftSibling != nullptr;

		// borrow from right sibling
		if (hasRightSibling && targetParent->children[childIndex + 1]->keys.size() >= degree) {
			removeTarget->keys.push_back(std::move(rightSibling->keys[0]));
			rightSibling->keys.erase(rightSibling->keys.begin());

			auto [rightSiblingParent, rightSiblingIndex] = findParent(root, rightSibling);

			if (rightSiblingIndex - 1 >= 0)
				rightSiblingParent->keys[rightSiblingIndex - 1] = rightSibling->keys[0];
		}
		// borrow from left sibling
		else if (hasLeftSibling && targetParent->children[childIndex - 1]->keys.size() >= degree) {
			removeTarget->keys.insert(removeTarget->keys.begin(), std::move(leftSibling->keys[leftSibling->keys.size() - 1]));
			leftSibling->keys.erase(leftSibling->keys.end() - 1);

			if (childIndex - 1 >= 0)
				targetParent->keys[childIndex - 1] = removeTarget->keys[0];
		}
		// merging 
		else {
			if (hasLeftSibling) {
				removeTarget = mergeLeafNodes(leftSibling, removeTarget);
				targetParent->children.erase(targetParent->children.begin() + childIndex - 1);
				targetParent->keys.erase(targetParent->keys.begin() + childIndex - 1);
			}
			else if (hasRightSibling) {
				removeTarget = mergeLeafNodes(removeTarget, rightSibling);
				targetParent->children.erase(targetParent->children.begin() + childIndex);
				targetParent->keys.erase(targetParent->keys.begin() + childIndex);
			}
		}
	}

	removeFixup(targetParent, key);
}

template<typename T, size_t degree>
void BPlusTree<T, degree>::removeFixup(BPlusTreeNode<T>* current, const T& key) {
	for (size_t i = 0, n = current->keys.size(); i < n; i++) {
		current->keys[i] = findMinElement(current->children[i + 1]);
	}

	if (current == root) {
		if (current->keys.empty()) {
			root = root->children[0];
			delete current;
		}
		return;
	}

	auto [currentParent, childIndex] = findParent(root, current);

	if (current->children.size() < degree) {
		auto leftSibling = findLeftSibling(current),
			rightSibling = findRightSibling(current);

		bool hasRightSibling = rightSibling != nullptr,
			hasLeftSibling = leftSibling != nullptr;

		// borrow from right sibling
		if (hasRightSibling && currentParent->children[childIndex + 1]->keys.size() >= degree) {
			current->keys.push_back(std::move(rightSibling->keys[0]));
			rightSibling->keys.erase(rightSibling->keys.begin());

			current->children.push_back(std::move(rightSibling->children[0]));
			rightSibling->children.erase(rightSibling->children.begin());

			if (childIndex >= 0)
				current->keys[childIndex] = findMinElement(current->children[current->children.size() - 1]);
		}
		// borrow from left sibling
		else if (hasLeftSibling && currentParent->children[childIndex - 1]->keys.size() >= degree) {
			current->keys.insert(current->keys.begin(), std::move(leftSibling->keys[leftSibling->keys.size() - 1]));
			leftSibling->keys.erase(leftSibling->keys.end() - 1);

			current->children.insert(current->children.begin(), std::move(leftSibling->children[leftSibling->children.size() - 1]));
			leftSibling->children.erase(leftSibling->children.end() - 1);

			if (childIndex - 1 >= 0)
				current->keys[childIndex - 1] = findMinElement(current->children[1]);
		}
		// merging 
		else {
			if (hasLeftSibling) {
				current = mergeInternalNodes(leftSibling, current);
			}
			else if (hasRightSibling) {
				current = mergeInternalNodes(current, rightSibling);
			}
		}
	}

	removeFixup(currentParent, key);
}

template<typename T, size_t degree>
BPlusTreeNode<T>* BPlusTree<T, degree>::mergeLeafNodes(BPlusTreeNode<T>* left, BPlusTreeNode<T>* right) {
	auto leftLeftSibling = findGlobalLeftSibling(left);

	if (leftLeftSibling)
		leftLeftSibling->sibling = right;

	std::reverse(right->keys.begin(), right->keys.end());
	std::move(left->keys.rbegin(), left->keys.rend(), std::back_inserter(right->keys));
	std::reverse(right->keys.begin(), right->keys.end());

	delete left;

	return right;
}

template<typename T, size_t degree>
BPlusTreeNode<T>* BPlusTree<T, degree>::mergeInternalNodes(BPlusTreeNode<T>* left, BPlusTreeNode<T>* right) {
	auto [parent, childLeftIndex] = findParent(root, left);

	right->keys.insert(right->keys.begin(), findMinElement(parent->children[childLeftIndex + 1]));
	parent->keys.erase(parent->keys.begin() + childLeftIndex);

	parent->children.erase(parent->children.begin() + childLeftIndex);

	std::reverse(right->keys.begin(), right->keys.end());
	std::move(left->keys.rbegin(), left->keys.rend(), std::back_inserter(right->keys));
	std::reverse(right->keys.begin(), right->keys.end());

	std::reverse(right->children.begin(), right->children.end());
	std::move(left->children.rbegin(), left->children.rend(), std::back_inserter(right->children));
	std::reverse(right->children.begin(), right->children.end());

	delete left;

	return right;
}

template<typename T, size_t degree>
T BPlusTree<T, degree>::findMinElement(BPlusTreeNode<T>* root) {
	while (!root->isLeaf) {
		root = root->children[0];
	}

	return root->keys[0];
}

template<typename T, size_t degree>
BPlusTreeNode<T>* BPlusTree<T, degree>::findLeftSibling(BPlusTreeNode<T>* node) {
	auto [parent, nodeIndex] = findParent(root, node);

	if (!parent)
		return nullptr;

	if (nodeIndex > 0) {
		return parent->children[nodeIndex - 1];
	}

	return nullptr;
}

template<typename T, size_t degree>
BPlusTreeNode<T>* BPlusTree<T, degree>::findRightSibling(BPlusTreeNode<T>* node) {
	auto [parent, nodeIndex] = findParent(root, node);

	if (!parent)
		return nullptr;

	if (parent->children.size() >= nodeIndex + 2) {
		return parent->children[nodeIndex + 1];
	}

	return nullptr;
}

template<typename T, size_t degree>
BPlusTreeNode<T>* BPlusTree<T, degree>::findGlobalLeftSibling(BPlusTreeNode<T>* current, BPlusTreeNode<T>* node) {
	if (current->isLeaf && current->sibling == node) {
		return current;
	}

	for (const auto& kid : current->children) {
		auto location = findGlobalLeftSibling(kid, node);
		if (location != nullptr)
			return location;
	}

	return nullptr;
}

template<typename T, size_t degree>
BPlusTreeNode<T>* BPlusTree<T, degree>::findGlobalLeftSibling(BPlusTreeNode<T>* node) {
	auto current = root;

	while (!current->isLeaf) {
		current = current->children[0];
	}

	if (current == node) {
		return nullptr;
	}

	return findGlobalLeftSibling(root, node);
}

template<typename T, size_t degree>
std::pair<BPlusTreeNode<T>*, size_t> BPlusTree<T, degree>::findParent(BPlusTreeNode<T>* current, BPlusTreeNode<T>* child) {
	for (size_t i = 0, n = current->children.size(); i < n; i++) {
		if (current->children[i] == child)
			return std::make_pair(current, i);
	}

	for (const auto& kid : current->children) {
		auto location = findParent(kid, child);
		if (location.second != -1)
			return location;
	}

	return { nullptr, -1 };
}

template<typename T, size_t degree>
void BPlusTree<T, degree>::getGraphInfo(BPlusTreeNode<T>* x, std::string& text) {
	if (x == nullptr)
		return;

	std::string currentNodeText = x->getText();
	std::stringstream stream;

	stream << "\"" << currentNodeText << "\";\n";

	if (x->sibling)
		stream << "\"" << currentNodeText << "\"" << " -> " << "\"" << x->sibling->getText() << "\";\n";

	for (const auto& child : x->children) {
		stream << "\"" << currentNodeText << "\"" << " -> " << "\"" << child->getText() << "\";\n";

		text += stream.str();
		stream.str(std::string());
		getGraphInfo(child, text);
	}

	text += stream.str();
	stream.str(std::string());
}

template<typename T, size_t degree>
std::string BPlusTree<T, degree>::getWebGraphviz(std::string graphName) {
	std::string graphText = "digraph " + graphName + " {\n";

	getGraphInfo(root, graphText);

	return graphText += "}";
}
