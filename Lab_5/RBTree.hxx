#include "RBTree.h"

template<typename T>
RBTree<T>::RBTree() {
	nill = new treeNode<T>();
	root = nill;
	root->parent = nill;
}

template<typename T>
RBTree<T>::RBTree(const std::initializer_list<T>& list) {
	nill = new treeNode<T>();
	root = nill;
	root->parent = nill;

	for (const auto& item : list)
		insert(item);

	persistentRoots.push(root);
}

template<typename T>
void RBTree<T>::leftRotate(treeNode<T>* x) {
	treeNode<T>* y = x->right;
	x->right = y->left;

	int alpha = x->left->descendants + 1, gamma = y->right->descendants + 1;
	if (x->left == nill) {
		alpha--;
	}
	if (y->right == nill) {
		gamma--;
	}

	x->descendants -= (gamma + 1);
	y->descendants += (alpha + 1);

	if (y->left != nill) {
		y->left->parent = x;
	}

	y->parent = x->parent;

	if (x->parent == nill) {
		root = y;
	}
	else {
		if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
	}
	y->left = x;
	x->parent = y;
}

template<typename T>
void RBTree<T>::rightRotate(treeNode<T>* y) {
	treeNode<T>* x = y->left;
	y->left = x->right;

	int alpha = x->left->descendants + 1, gamma = y->right->descendants + 1;
	if (x->left == nill) {
		alpha--;
	}
	if (y->right == nill) {
		gamma--;
	}

	x->descendants += (gamma + 1);
	y->descendants -= (alpha + 1);

	if (x->right != nill) {
		x->right->parent = y;
	}

	x->parent = y->parent;

	if (y->parent == nill) {
		root = x;
	}
	else {
		if (y == y->parent->right) {
			y->parent->right = x;
		}
		else {
			y->parent->left = x;
		}
	}
	x->right = y;
	y->parent = x;
}

template<typename T>
void RBTree<T>::insertFixup(treeNode<T>* z) {
	treeNode<T>* y;

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

template<typename T>
void RBTree<T>::insertFixupPersistent(treeNode<T>* z) {
	treeNode<T>* y;

	while (z->parent->color == RED) {
		if (z->parent == z->parent->parent->left) {

			if (z->parent->parent->right != nill) {
				auto newUncle = copy(z->parent->parent->right);
				newUncle->parent = z->parent->parent;
				z->parent->parent->right = newUncle;
			}

			y = z->parent->parent->right;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) {
					if (z->left != nill) {
						auto newLeft = copy(z->left);
						newLeft->parent = z;
						z->left = newLeft;
					}

					z = z->parent;
					leftRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				if (z->parent->right != nill) {
					auto newRight = copy(z->parent->right);
					newRight->parent = z->parent;
					z->parent->right = newRight;
				}

				rightRotate(z->parent->parent);
			}
		}
		else {
			if (z->parent->parent->left != nill) {
				auto newUncle = copy(z->parent->parent->left);
				newUncle->parent = z->parent->parent;
				z->parent->parent->left = newUncle;
			}

			y = z->parent->parent->left;
			if (y->color == RED) {
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					if (z->right != nill) {
						auto newRight = copy(z->right);
						newRight->parent = z;
						z->right = newRight;
					}

					z = z->parent;
					rightRotate(z);
				}
				z->parent->color = BLACK;
				z->parent->parent->color = RED;

				if (z->parent->left != nill) {
					auto newLeft = copy(z->parent->left);
					newLeft->parent = z->parent;
					z->parent->left = newLeft;
				}

				leftRotate(z->parent->parent);
			}
		}
	}

	root->color = BLACK;
}

template<typename T>
void RBTree<T>::removeFixup(treeNode<T>* x) {
	while (x != root && x->color == BLACK) {
		treeNode<T>* w;
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

template<typename T>
void RBTree<T>::removeFixupPersistent(treeNode<T>* x) {
	while (x != root && x->color == BLACK) {
		treeNode<T>* w;
		if (x == x->parent->left) {
			if (x->parent->right != nill) {
				auto newW = copy(x->parent->right);
				newW->parent = x->parent;
				x->parent->right = newW;
				w = newW;
			}
			else
				w = nill;

			//w = x->parent->right;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;

				if (w->left != nill) {
					auto newLeft = copy(w->left);
					w->left = newLeft;
					newLeft->parent = w;
				}

				leftRotate(x->parent);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->right->color == BLACK) {

					if (w->left != nill) {
						auto newLeft = copy(w->left);
						w->left = newLeft;
						newLeft->parent = w;
					}

					if (w->left->right != nill) {
						auto newRight = copy(w->left->right);
						w->left->right = newRight;
						newRight->parent = w->left;
					}

					w->left->color = BLACK;
					w->color = RED;
					rightRotate(w);
					w = x->parent->right;
				}
				w->color = x->parent->color;

				if (w->left != nill) {
					auto newLeft = copy(w->left);
					w->left = newLeft;
					newLeft->parent = w;
				}

				if (w->right != nill) {
					auto newRight = copy(w->right);
					w->right = newRight;
					newRight->parent = w;
				}

				x->parent->color = BLACK;
				w->right->color = BLACK;
				leftRotate(x->parent);
				x = root;
			}
		}
		else {
			if (x->parent->left != nill) {
				auto newW = copy(x->parent->left);
				newW->parent = x->parent;
				x->parent->left = newW;
				w = newW;
			}
			else
				w = nill;
			//w = x->parent->left;
			if (w->color == RED) {
				w->color = BLACK;
				x->parent->color = RED;

				if (w->right != nill) {
					auto newRight = copy(w->right);
					w->right = newRight;
					newRight->parent = w;
				}

				rightRotate(x->parent);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK) {
				w->color = RED;
				x = x->parent;
			}
			else {
				if (w->left->color == BLACK) {

					if (w->right != nill) {
						auto newRight = copy(w->right);
						w->right = newRight;
						newRight->parent = w;
					}

					if (w->right->left != nill) {
						auto newLeft = copy(w->right->left);
						w->right->left = newLeft;
						newLeft->parent = w->right;
					}

					w->right->color = BLACK;
					w->color = RED;
					leftRotate(w);
					w = x->parent->left;
				}
				w->color = x->parent->color;

				if (w->right != nill) {
					auto newRight = copy(w->right);
					w->right = newRight;
					newRight->parent = w;
				}

				if (w->left != nill) {
					auto newLeft = copy(w->left);
					w->left = newLeft;
					newLeft->parent = w;
				}

				x->parent->color = BLACK;
				w->left->color = BLACK;
				rightRotate(x->parent);
				x = root;
			}
		}
	}
	x->color = BLACK;
}

template<typename T>
treeNode<T>* RBTree<T>::treeSuccessor(treeNode<T>* x) {
	if (x->right != nill) {
		return treeMinimum(x->right);
	}
	treeNode<T>* y = x->parent;
	while (y != nill && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

template<typename T>
treeNode<T>* RBTree<T>::treeSuccessorPersistent(treeNode<T>* x) {
	if (x->right != nill) {

		return treeMinimumPersistent(x->right, x);
	}
	treeNode<T>* y = x->parent;
	while (y != nill && x == y->right) {
		x = y;
		y = y->parent;
	}
	return y;
}

template<typename T>
treeNode<T>* RBTree<T>::treeMinimum(treeNode<T>* x) {
	while (x->left != nill) {
		x = x->left;
	}
	return x;
}

template<typename T>
treeNode<T>* RBTree<T>::treeMinimumPersistent(treeNode<T>* x, treeNode<T>* pre) {
	auto newPre = new treeNode<T>();

	if (x->left != nill) {
		auto newX = copy(x);
		newX->parent = pre;

		pre->right = newX;

		x = newX;
		newPre = x;
		x = x->left;
	}

	while (x->left != nill) {
		auto newX = copy(x);
		newX->parent = newPre;
		x = newX;
		x = x->left;
	}

	if (x != nill) {
		auto newX = copy(x);
		newX->parent = newPre;
		if (x == newPre->left) {
			newPre->left = newX;
		}
		else {
			newPre->right = newX;
		}
		x = newX;
	}

	return x;
}

template<typename T>
treeNode<T>* RBTree<T>::treeMaximum(treeNode<T>* x) {
	while (x->right != nill) {
		x = x->right;
	}
	return x;
}

template<typename T>
treeNode<T>* RBTree<T>::treeSearch(treeNode<T>* x, const T& key) {
	if (x == nill || key == x->key) {
		return x;
	}
	if (key < x->key)
		return treeSearch(x->left, key);
	else
		return treeSearch(x->right, key);
}

template<typename T>
void RBTree<T>::insert(const T& item) {
	treeNode<T>* z = new treeNode<T>();
	z->key = item;

	treeNode<T>* y = nill;
	treeNode<T>* x = root;

	while (x != nill) {
		y = x;
		x->descendants++;
		if (z->key < x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}

	z->parent = y;
	if (y == nill) {
		root = z;
	}
	else {
		if (z->key < y->key) {
			y->left = z;
		}
		else {
			y->right = z;
		}
	}

	z->left = nill;
	z->right = nill;
	z->color = RED;

	insertFixup(z);
}

template<typename T>
void RBTree<T>::insertPersistent(const T& item) {
	if (root == nill) {
		insert(item);
		persistentRoots.push(root);
		return;
	}
	else {
		auto newRoot = copy(root);
		persistentRoots.push(newRoot);
		root = newRoot;
	}

	treeNode<T>* z = new treeNode<T>();
	z->key = item;

	treeNode<T>* y = nill;
	treeNode<T>* x = root;

	while (x != nill) {
		if (x != root) {
			auto newX = copy(x);
			newX->parent = y;
			if (y->left == x) {
				y->left = newX;
			}
			else {
				y->right = newX;
			}

			x = newX;
		}
		y = x;
		x->descendants++;

		if (z->key <= x->key) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}

	z->parent = y;
	z->generation = y->generation;
	if (y == nill) {
		root = z;
	}
	else {
		if (z->key <= y->key) {
			y->left = z;
		}
		else {
			y->right = z;
		}
	}

	z->left = nill;
	z->right = nill;
	z->color = RED;

	insertFixupPersistent(z);
}

template<typename T>
size_t RBTree<T>::size() const {
	if (root != nill)
		return root->descendants + 1;
	else
		return 0;
}

template<typename T>
treeNode<T>* RBTree<T>::treeSearch(const T& key) {
	return treeSearch(root, key);
}

template<typename T>
treeNode<T>* RBTree<T>::treeSearchPersistent(const T& key) {
	if (root != nill) {
		auto newRoot = copy(root);
		persistentRoots.push(newRoot);
		root = newRoot;
	}

	treeNode<T>* pre = nill;
	treeNode<T>* iter = root;

	while (iter->key != key && iter != nill) {
		if (iter != root) {
			auto newIter = copy(iter);
			newIter->parent = pre;
			if (pre->left == iter) {
				pre->left = newIter;
			}
			else {
				pre->right = newIter;
			}

			iter = newIter;
		}
		pre = iter;

		if (key < iter->key) {
			iter = iter->left;
		}
		else {
			iter = iter->right;
		}
	}

	if (iter != root && iter != nill) {
		auto newIter = copy(iter);
		newIter->parent = pre;
		if (pre->left == iter) {
			pre->left = newIter;
		}
		else {
			pre->right = newIter;
		}

		iter = newIter;
	}

	return iter;
}

template<typename T>
T& RBTree<T>::operator[](size_t index) {
	if (index < 0 || index > size()) {
		throw std::invalid_argument("Out of range!");
	}

	treeNode<T>* current = root;
	while ((current->left->descendants + (current->left != nill)) != index) {
		if (current->left->descendants >= index) {
			current = current->left;
		}
		else {
			index -= (current->left->descendants + 1 + (current->left != nill));
			current = current->right;
		}
	}
	return current->key;
}

template<typename T>
treeNode<T>* RBTree<T>::remove(const T& item) {
	treeNode<T>* z = treeSearch(root, item);
	treeNode<T>* decreaser;

	if (z == nill)
		throw std::invalid_argument("There's no element with such parameters.");

	treeNode<T>* x, * y;

	if (z->left == nill || z->right == nill)
		y = z;
	else
		y = treeSuccessor(z);

	decreaser = y->parent;

	if (y->left != nill)
		x = y->left;
	else
		x = y->right;

	x->parent = y->parent;

	if (y->parent == nill) {
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

	while (decreaser != nill) {
		decreaser->descendants--;
		decreaser = decreaser->parent;
	}

	if (y->color == BLACK) {
		removeFixup(x);
	}

	return y;
}

template<typename T>
treeNode<T>* RBTree<T>::removePersistent(const T& item) {
	treeNode<T>* z = treeSearchPersistent(item);
	treeNode<T>* decreaser;

	if (z == nill)
		throw std::invalid_argument("There's no element with such parameters.");

	treeNode<T>* x, * y;

	if (z->left == nill || z->right == nill)
		y = z;
	else {
		y = treeSuccessorPersistent(z);
	}

	decreaser = y->parent;

	if (y->left != nill) {

		if (y->left != nill) {
			auto newLeft = copy(y->left);
			y->left = newLeft;
		}

		x = y->left;
	}
	else {

		if (y->right != nill) {
			auto newRight = copy(y->right);
			y->right = newRight;
		}

		x = y->right;
	}

	x->parent = y->parent;

	if (y->parent == nill) {
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

	while (decreaser != nill) {
		decreaser->descendants--;
		decreaser = decreaser->parent;
	}

	if (y->color == BLACK) {
		removeFixupPersistent(x);
	}

	return y;
}

template<typename T>
void RBTree<T>::clear(treeNode<T>* x, int gen) {
	if (x->left != nill)
		clear(x->left, gen);
	if (x->right != nill)
		clear(x->right, gen);
	if (x->generation == gen)
		delete x;
}

template<typename T>
void RBTree<T>::skipBack() {
	persistentRoots.pop();
	clear(root, root->generation);

	root = persistentRoots.top();
}

template<typename T>
void RBTree<T>::inorderTreeWalk(treeNode<T>* x, int depth) {
	if (x != nill) {
		inorderTreeWalk(x->left, depth + 1);
		std::cout << x->key << " Color: " << (x->color ? "BLACK" : "RED") << ";  nodeDepth = " << depth << ";  Descendants: " << x->descendants << std::endl;
		inorderTreeWalk(x->right, depth + 1);
	}
}

template<typename U>
std::ostream& operator<<(std::ostream& os, RBTree<U>& tree) {
	tree.inorderTreeWalk(tree.root);
	return os;
}

template<typename T>
void RBTree<T>::getGraphInfo(treeNode<T>* x, std::string& text) {
	if (x != nill) {
		if (x->color == RED)
			text += "\"" + std::to_string(x->key) + "\" [color = \"red\"];\n";
		else if (x == root)
			text += "\"" + std::to_string(x->key) + "\";\n";
		if (x->left != nill)
			text += "\"" + std::to_string(x->key) + "\"" + " -> " + "\"" + std::to_string(x->left->key) + "\";\n";
		if (x->right != nill)
			text += "\"" + std::to_string(x->key) + "\"" + " -> " + "\"" + std::to_string(x->right->key) + "\";\n";

		getGraphInfo(x->left, text);
		getGraphInfo(x->right, text);
	}
}

template<typename T>
std::string RBTree<T>::getWebGraphviz(std::string graphName) {
	std::string graphText = "digraph " + graphName + " {\n";

	getGraphInfo(root, graphText);

	return graphText += "}";
}
