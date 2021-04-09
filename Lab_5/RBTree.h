#pragma once

#include <iostream>
#include <iomanip>
#include <stack>
#include <string>

enum Color { RED, BLACK };

template <typename T>
struct treeNode;

template <typename T>
struct treeNode {
	T key;
	Color color = BLACK;
	treeNode* parent = nullptr;
	treeNode* left = nullptr;
	treeNode* right = nullptr;
	size_t descendants = 0;
	int generation = 0; //defines persistent set generation for this node to delete it corectly while skipping back

	treeNode() {};
};

template <typename T>
treeNode<T>* copy(treeNode<T>* origin) {
	auto destination = new treeNode<T>();

	destination->key = origin->key;
	destination->color = origin->color;
	destination->parent = origin->parent;
	destination->left = origin->left;
	destination->right = origin->right;
	destination->generation = origin->generation + 1;
	destination->descendants = origin->descendants;

	return destination;
}

template <typename T>
class RBTree {
	treeNode<T>* nill;
	treeNode<T>* root;

	void leftRotate(treeNode<T>* x);
	void rightRotate(treeNode<T>* x);

	void insertFixup(treeNode<T>* z);
	void insertFixupPersistent(treeNode<T>* z);

	void removeFixup(treeNode<T>* x);
	void removeFixupPersistent(treeNode<T>* x);

	treeNode<T>* treeSuccessor(treeNode<T>* x);
	treeNode<T>* treeSuccessorPersistent(treeNode<T>* x);

	treeNode<T>* treeMinimum(treeNode<T>* x);
	treeNode<T>* treeMinimumPersistent(treeNode<T>* x, treeNode<T>* pre);

	treeNode<T>* treeMaximum(treeNode<T>* x);
	treeNode<T>* treeSearch(treeNode<T>* x, const T& key);

	void getGraphInfo(treeNode<T>* x, std::string& text);

public:
	std::stack<treeNode<T>*> persistentRoots;

	RBTree();
	RBTree(const std::initializer_list<T>& list);

	void skipBack();
	void clear(treeNode<T>* x, int gen);

	void inorderTreeWalk(treeNode<T>* x, int depth = 0);

	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, RBTree<U>& tree);

	std::string getWebGraphviz(std::string graphName = "G");

	void insert(const T& item);
	void insertPersistent(const T& item);

	size_t size() const;

	treeNode<T>* treeSearch(const T& key);
	treeNode<T>* treeSearchPersistent(const T& key);

	T& operator [](size_t index);
	treeNode<T>* remove(const T& item);
	treeNode<T>* removePersistent(const T& item);
};

#include "RBTree.hxx"
