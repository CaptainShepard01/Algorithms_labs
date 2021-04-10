#pragma once

#include <iostream>
#include <iomanip>
#include <stack>
#include <string>
#include <vector>
#include "WorldMap.h"

enum Color { RED, BLACK };

template <typename T>
struct treeNode;

template <typename T>
struct treeNode {
	int ID;
	static int number;

	T key;
	Color color = BLACK;
	treeNode* parent = nullptr;
	treeNode* left = nullptr;
	treeNode* right = nullptr;
	size_t descendants = 0;
	int generation = 0; //defines persistent set generation for this node to delete it corectly while skipping back

	treeNode():ID(number++) {};
};

template<typename T>
int treeNode<T>::number = 0;

template <typename T>
class RBTree {
	treeNode<T>* nill;
	treeNode<T>* root;

	treeNode<T>* copy(treeNode<T>* origin);

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
	void getGraphInfoPersistent(treeNode<T>* x, std::string& text, int gen);

public:
	std::vector<treeNode<T>*> persistentRoots;

	RBTree();
	RBTree(const std::initializer_list<T>& list, bool isPersistent = false);

	void skipBack();
	void clear(treeNode<T>* x, int gen);

	void inorderTreeWalk(treeNode<T>* x, int depth = 0);

	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, RBTree<U>& tree);

	std::string getWebGraphviz(std::string graphName = "G");
	std::string getWebGraphvizPersistent(std::string graphName = "G");

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
