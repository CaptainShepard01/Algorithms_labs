#include "BPTree.h"

template<typename T, size_t degree>
BPTree<T, degree>::BPTree() {
	root = new BPTreeNode<T>();
	root->isLeaf = true;
}

template<typename T, size_t degree>
BPTree<T, degree>::BPTree(const std::initializer_list<T>& list) {
	root = new BPTreeNode<T>();
	root->isLeaf = true;

	for (const auto& item : list) {
		insert(item);
	}
}


template<typename T, size_t degree>
std::pair<BPTreeNode<T>*, size_t> BPTree<T, degree>::findNode(BPTreeNode<T>* current, const T& key) {
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
std::pair<BPTreeNode<T>*, size_t> BPTree<T, degree>::findNode(const T& key) {
	return findNode(root, key);
}

template<typename T, size_t degree>
void BPTree<T, degree>::splitChildNode(BPTreeNode<T>* parent, size_t childIndex) {
	BPTreeNode<T>* newNode = new BPTreeNode<T>;
	BPTreeNode<T>* toSplit = parent->children[childIndex];

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
void BPTree<T, degree>::insertGeneral(BPTreeNode<T>* current, const T& key) {
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
void BPTree<T, degree>::insert(const T& key) {
	BPTreeNode<T>* current = root;

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

	insertGeneral(current, key);

	if (current->keys.size() == 2 * degree) {
		recursiveSplit(current);
	}
}

template<typename T, size_t degree>
void BPTree<T, degree>::recursiveSplit(BPTreeNode<T>* current) {
	if (current->keys.size() != 2 * degree)
		return;

	if (current == root) {
		BPTreeNode<T>* newRoot = new BPTreeNode<T>();
		newRoot->children.push_back(current);

		root = newRoot;
		splitChildNode(newRoot, 0);

		return;
	}
	auto [parent, index] = findParent(root, current);

	splitChildNode(parent, index);

	recursiveSplit(parent);
}

template<typename T, size_t degree>
void BPTree<T, degree>::deleteData(const T& key) {
	auto [removeTarget, removeTargetIndex] = findNode(key);
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
		auto leftSibling_ = leftSibling(removeTarget),
			rightSibling_ = rightSibling(removeTarget);

		bool hasRightSibling = rightSibling_ != nullptr,
			hasLeftSibling = leftSibling_ != nullptr;

		if (hasRightSibling && targetParent->children[childIndex + 1]->keys.size() >= degree) {
			removeTarget->keys.push_back(std::move(rightSibling_->keys[0]));
			rightSibling_->keys.erase(rightSibling_->keys.begin());

			auto [rightSiblingParent, rightSiblingIndex] = findParent(root, rightSibling_);

			if (rightSiblingIndex - 1 >= 0)
				rightSiblingParent->keys[rightSiblingIndex - 1] = rightSibling_->keys[0];
		}
		// borrow from left sibling
		else if (hasLeftSibling && targetParent->children[childIndex - 1]->keys.size() >= degree) {
			removeTarget->keys.insert(removeTarget->keys.begin(), std::move(leftSibling_->keys[leftSibling_->keys.size() - 1]));
			leftSibling_->keys.erase(leftSibling_->keys.end() - 1);

			if (childIndex - 1 >= 0)
				targetParent->keys[childIndex - 1] = removeTarget->keys[0];
		}
		// merging 
		else {
			if (hasLeftSibling) {
				removeTarget = mergeLeafs(leftSibling_, removeTarget);
				targetParent->children.erase(targetParent->children.begin() + childIndex - 1);
				targetParent->keys.erase(targetParent->keys.begin() + childIndex - 1);
			}
			else if (hasRightSibling) {
				removeTarget = mergeLeafs(removeTarget, rightSibling_);
				targetParent->children.erase(targetParent->children.begin() + childIndex);
				targetParent->keys.erase(targetParent->keys.begin() + childIndex);
			}
		}
	}

	deleteFixup(targetParent, key);
}

template<typename T, size_t degree>
void BPTree<T, degree>::deleteFixup(BPTreeNode<T>* current, const T& key) {
	for (size_t i = 0, n = current->keys.size(); i < n; i++) {
		current->keys[i] = findMin(current->children[i + 1]);
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
		auto leftSibling_ = leftSibling(current),
			rightSibling_ = rightSibling(current);

		bool hasRightSibling = rightSibling_ != nullptr,
			hasLeftSibling = leftSibling_ != nullptr;

		// borrow from right sibling
		if (hasRightSibling && currentParent->children[childIndex + 1]->keys.size() >= degree) {
			current->keys.push_back(std::move(rightSibling_->keys[0]));
			rightSibling_->keys.erase(rightSibling_->keys.begin());

			current->children.push_back(std::move(rightSibling_->children[0]));
			rightSibling_->children.erase(rightSibling_->children.begin());

			if (childIndex >= 0)
				current->keys[childIndex] = findMin(current->children[current->children.size() - 1]);
		}
		else if (hasLeftSibling && currentParent->children[childIndex - 1]->keys.size() >= degree) {
			current->keys.insert(current->keys.begin(), std::move(leftSibling_->keys[leftSibling_->keys.size() - 1]));
			leftSibling_->keys.erase(leftSibling_->keys.end() - 1);

			current->children.insert(current->children.begin(), std::move(leftSibling_->children[leftSibling_->children.size() - 1]));
			leftSibling_->children.erase(leftSibling_->children.end() - 1);

			if (childIndex - 1 >= 0)
				current->keys[childIndex - 1] = findMin(current->children[1]);
		}
		// merging 
		else {
			if (hasLeftSibling) {
				current = mergeDividers(leftSibling_, current);
			}
			else if (hasRightSibling) {
				current = mergeDividers(current, rightSibling_);
			}
		}
	}

	deleteFixup(currentParent, key);
}

template<typename T, size_t degree>
BPTreeNode<T>* BPTree<T, degree>::mergeLeafs(BPTreeNode<T>* left, BPTreeNode<T>* right) {
	auto leftLeftSibling = leftSiblingInTree(left);

	if (leftLeftSibling)
		leftLeftSibling->sibling = right;

	std::reverse(right->keys.begin(), right->keys.end());
	std::move(left->keys.rbegin(), left->keys.rend(), std::back_inserter(right->keys));
	std::reverse(right->keys.begin(), right->keys.end());

	delete left;

	return right;
}

template<typename T, size_t degree>
BPTreeNode<T>* BPTree<T, degree>::mergeDividers(BPTreeNode<T>* left, BPTreeNode<T>* right) {
	auto [parent, childIndexLeft] = findParent(root, left);

	right->keys.insert(right->keys.begin(), findMin(parent->children[childIndexLeft + 1]));
	parent->keys.erase(parent->keys.begin() + childIndexLeft);

	parent->children.erase(parent->children.begin() + childIndexLeft);

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
T BPTree<T, degree>::findMin(BPTreeNode<T>* root) {
	while (!root->isLeaf) {
		root = root->children[0];
	}

	return root->keys[0];
}

template<typename T, size_t degree>
BPTreeNode<T>* BPTree<T, degree>::leftSibling(BPTreeNode<T>* node) {
	auto [parent, nodeIndex] = findParent(root, node);

	if (!parent)
		return nullptr;

	if (nodeIndex > 0) {
		return parent->children[nodeIndex - 1];
	}

	return nullptr;
}

template<typename T, size_t degree>
BPTreeNode<T>* BPTree<T, degree>::rightSibling(BPTreeNode<T>* node) {
	auto [parent, nodeIndex] = findParent(root, node);

	if (!parent)
		return nullptr;

	if (parent->children.size() >= nodeIndex + 2) {
		return parent->children[nodeIndex + 1];
	}

	return nullptr;
}

template<typename T, size_t degree>
BPTreeNode<T>* BPTree<T, degree>::leftSiblingInTree(BPTreeNode<T>* current, BPTreeNode<T>* node) {
	if (current->isLeaf && current->sibling == node) {
		return current;
	}

	for (const auto& kid : current->children) {
		auto location = leftSiblingInTree(kid, node);
		if (location != nullptr)
			return location;
	}

	return nullptr;
}

template<typename T, size_t degree>
BPTreeNode<T>* BPTree<T, degree>::leftSiblingInTree(BPTreeNode<T>* node) {
	auto current = root;

	while (!current->isLeaf) {
		current = current->children[0];
	}

	if (current == node) {
		return nullptr;
	}

	return leftSiblingInTree(root, node);
}

template<typename T, size_t degree>
std::pair<BPTreeNode<T>*, size_t> BPTree<T, degree>::findParent(BPTreeNode<T>* current, BPTreeNode<T>* child) {
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
void BPTree<T, degree>::getGraphInfo(BPTreeNode<T>* x, std::string& text) {
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
std::string BPTree<T, degree>::getWebGraphviz(std::string graphName) {
	std::string graphText = "digraph " + graphName + " {\n";

	getGraphInfo(root, graphText);

	return graphText += "}";
}