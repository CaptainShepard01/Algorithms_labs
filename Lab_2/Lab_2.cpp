#include <iostream>
#include <vector>
#include <string>


enum Color { RED, BLACK };

template <typename T>
struct treeNode {
	T key;
	Color color = BLACK;
	treeNode* parent = nullptr;
	treeNode* left = nullptr;
	treeNode* right = nullptr;
};

template <typename T>
class RBTree;


template <typename T>
class RBTree {
private:
	treeNode<T>* root;

	void leftRotate(treeNode<T>* x) {
		treeNode<T>* y = x->right;
		x->right = y->left;

		if (y->left != nullptr) {
			y->left->parent = x;
		}

		y->parent = x->parent;

		if (x->parent == nullptr) {
			root = y;
		}
		else {
			if (x == x->parent->left) {
				x->parent->left = y;
			}
			else {
				x->parent->left = y;
			}
		}
		y->left = x;
		x->parent = y;
	}

	void rightRotate(treeNode<T>* x) {
		treeNode<T>* y = x->left;
		x->left = y->right;

		if (y->right != nullptr) {
			y->right->parent = x;
		}

		y->parent = x->parent;

		if (x->parent == nullptr) {
			root = y;
		}
		else {
			if (x == x->parent->right) {
				x->parent->right = y;
			}
			else {
				x->parent->right = y;
			}
		}
		y->right = x;
		x->parent = y;
	}

	void insertFixup(treeNode<T>* z) {
		treeNode<T>* y = new treeNode<T>;

		while (z->parent->color == RED) {
			if (z->parent == z->parent->parent->left) {
				y = z->parent->parent->right;
				if (y->color == RED) {
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->right) {
						z = z->parent;
						leftRotate(z);
					}
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					rightRotate(z->parent->parent);
				}
			}
			else {
				y = z->parent->parent->left;
				if (y->color == RED) {
					z->parent->color = BLACK;
					y->color = BLACK;
					z->parent->parent->color = RED;
					z = z->parent->parent;
				}
				else {
					if (z == z->parent->left) {
						z = z->parent;
						rightRotate(z);
					}
					z->parent->color = BLACK;
					z->parent->parent->color = RED;
					leftRotate(z->parent->parent);
				}
			}
		}

		root->color = BLACK;
	}

	void removeFixup(treeNode<T>* x) {
		while (x != root && x->color == BLACK) {
			treeNode<T>* w = new treeNode<T>;
			if (x == x->parent->left) {
				w = x->parent->right;
				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					leftRotate(x->parent);
					w = x->parent->right;
				}
				if (w->left->color == BLACK && w->right->color == BLACK) {
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->right->color == BLACK) {
						w->left->color = BLACK;
						w->color = RED;
						rightRotate(w);
						w = x->parent->right;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					leftRotate(x->parent);
					x = root;
				}
			}
			else {
				w = x->parent->left;
				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = RED;
					rightRotate(x->parent);
					w = x->parent->left;
				}
				if (w->right->color == BLACK && w->left->color == BLACK) {
					w->color = RED;
					x = x->parent;
				}
				else {
					if (w->left->color == BLACK) {
						w->right->color = BLACK;
						w->color = RED;
						leftRotate(w);
						w = x->parent->left;
					}
					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->left->color = BLACK;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = BLACK;
	}

	treeNode<T>* treeSuccessor(treeNode<T>* x) {
		if (x->right != nullptr) {
			return treeMinimum(x->right);
		}
		treeNode<T>* y = x->parent;
		while (y != nullptr && x == x->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	treeNode<T>* treeMinimum(treeNode<T>* x) {
		while (x->left != nullptr) {
			x = x->left;
		}
		return x;
	}

	treeNode<T>* treeMaximum(treeNode<T>* x) {
		while (x->right != nullptr) {
			x = x->right;
		}
		return x;
	}

public:
	RBTree() {}

	friend std::ostream& operator<< (std::ostream& os, const RBTree<T>& tree);

	treeNode<T>* getRoot() {
		return root;
	}

	void inorderTreeWalk(treeNode<T>* x) {
		if (x != nullptr) {
			inorderTreeWalk(x->left);
			std::cout << x->key << ' ';
			inorderTreeWalk(x->right);
		}
	}

	void insert(const T& item) {
		treeNode<T>* z = new treeNode<T>;
		z->key = item;

		treeNode<T>* y = nullptr;
		treeNode<T>* x = root;

		while (x != nullptr) {
			y = x;
			if (z->key < x->key) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		z->parent = y;
		if (y == nullptr) {
			root = z;
		}
		else {
			if(z->key<y->key){
				y->left = z;
			}
			else {
				y->right = z;
			}
		}

		z->left = nullptr;
		z->right = nullptr;
		z->color = RED;
		if (z == root || z->parent == root || z->parent->parent==root || z->parent->parent->left==nullptr|| z->parent->parent->right == nullptr) {
			return;
		}
		insertFixup(z);
	};

	treeNode<T>* treeSearch(treeNode<T>* x, const T& key) {
		if (x == nullptr || key == x->key) {
			return x;
		}
		if (key < x->key)
			return treeSearch(x->left, key);
		else
			return treeSearch(x->right, key);
	}

	treeNode<T>* remove(const T& item) {
		treeNode<T>* z = treeSearch(getRoot(), item);

		treeNode<T>* y = new treeNode<T>;
		treeNode<T>* x = new treeNode<T>;

		if (z->left != nullptr || z->right != nullptr) {
			y = z;
		}
		else {
			y = treeSuccessor(z);
		}

		if (y->left != nullptr) {
			x = y->left;
		}
		else {
			x = y->right;
		}

		x->parent = y->parent;

		if (y->parent == nullptr) {
			root = x;
		}
		else {
			if (y == y->parent->left) {
				y->parent->left = x;
			}
			else {
				y->parent->right = x;
			}
		}

		if (y != z) {
			z->key = y->key;
		}

		if (y->color == BLACK) {
			removeFixup(x);
		}

		return y;
	}

	treeNode<T>* remove(treeNode<T>* z) {
		treeNode<T>* y = new treeNode<T>;
		treeNode<T>* x = new treeNode<T>;

		if (z->left == nullptr && z->right == nullptr) {
			y = z;
		}
		else {
			y = treeSuccessor(z);
		}

		if (y->left != nullptr) {
			x = y->left;
		}
		else {
			x = y->right;
		}

		x->parent = y->parent;

		if (y->parent == nullptr) {
			root = x;
		}
		else {
			if (y == y->parent->left) {
				y->parent->left = x;
			}
			else {
				y->parent->right = x;
			}
		}

		if (y != z) {
			z->key = y->key;
		}

		if (y->color == BLACK) {
			removeFixup(x);
		}

		return y;
	}
};

template <class T>
std::ostream& operator<<(std::ostream& os, const RBTree<T>& tree) {
	tree.inorderTreeWalk();
	return os;
}

int main()
{
	RBTree<int> testTree;

	testTree.insert(5);
	testTree.insert(5);
	testTree.insert(5);
	testTree.insert(5);
	testTree.insert(3);
	testTree.insert(2);
	testTree.insert(0);
	testTree.insert(5);


	testTree.inorderTreeWalk(testTree.getRoot());

	std::cout << std::endl;

	testTree.remove(3);

	testTree.inorderTreeWalk(testTree.getRoot());


	//std::cout << testTree << std::endl;
}