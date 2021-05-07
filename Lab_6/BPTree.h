#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include "../Lab_5/WorldMap.h"

template <typename T>
class BPTreeNode {
	static size_t N;

	const size_t ID;
	bool isLeaf = false;
	BPTreeNode<T>* sibling = nullptr;

	std::vector<T> keys;
	std::vector<BPTreeNode<T>*> children;

	template <typename T, const size_t>
	friend class BPTree;


public:
	std::string getText();

public:
	BPTreeNode();
};

template <typename T>
size_t BPTreeNode<T>::N = 0;

template<typename T>
BPTreeNode<T>::BPTreeNode<T>()
	: ID(N++) {}

template<typename T>
std::string BPTreeNode<T>::getText() {
	std::stringstream result;

	for (const auto& key : keys) {
		result << key << " ";
	}

	result << "ID : " << ID;

	return result.str();
}

template <typename T, const size_t degree>
class BPTree {
	BPTreeNode<T>* root;

	void getGraphInfo(BPTreeNode<T>* x, std::string& text);

	void splitChildNode(BPTreeNode<T>* parent, size_t childIndex);

	void insertGeneral(BPTreeNode<T>* current, const T& key);

	std::pair<BPTreeNode<T>*, size_t> findNode(BPTreeNode<T>* current, const T& key);

	std::pair<BPTreeNode<T>*, size_t>  findParent(BPTreeNode<T>* current, BPTreeNode<T>* child);

	T findMin(BPTreeNode<T>* root);

	BPTreeNode<T>* leftSibling(BPTreeNode<T>* node);

	BPTreeNode<T>* leftSiblingInTree(BPTreeNode<T>* current, BPTreeNode<T>* node);

	BPTreeNode<T>* leftSiblingInTree(BPTreeNode<T>* node);

	BPTreeNode<T>* rightSibling(BPTreeNode<T>* node);

	void recursiveSplit(BPTreeNode<T>* current);

	BPTreeNode<T>* mergeLeafs(BPTreeNode<T>* left, BPTreeNode<T>* right);

	BPTreeNode<T>* mergeDividers(BPTreeNode<T>* left, BPTreeNode<T>* right);

	void deleteFixup(BPTreeNode<T>* current, const T& key);
public:
	BPTree();
	BPTree(const std::initializer_list<T>& list);

	void insert(const T& key);

	std::pair<BPTreeNode<T>*, size_t> findNode(const T& key);

	void deleteData(const T& key);

	std::string getWebGraphviz(std::string graphName = "G");
};

#include "BPTree.hxx"