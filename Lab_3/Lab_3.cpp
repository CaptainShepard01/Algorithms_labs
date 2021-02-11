#include <iostream>
#include <exception>


struct WorldMap {
	std::string country;
	std::string city;

	bool operator <(const WorldMap& rhs) const {
		return this->city < rhs.city;
	}

	bool operator >(const WorldMap& rhs)const {
		return this->city > rhs.city;
	}

	bool operator ==(const WorldMap& rhs)const {
		return this->city == rhs.city;
	}
};

std::ostream& operator<<(std::ostream& os, const WorldMap& v) {
	os << '{';

	if (v.city != "") {
		os << " Country: " << v.country << ", City: " << v.city;
	}

	os << " }";
	return os;
}


template <typename T>
struct treeNode {
	T key;

	treeNode* parent;
	treeNode* left = nullptr;
	treeNode* right = nullptr;
};

template <typename T>
class SplayTree {
private:
	treeNode<T>* nill = new treeNode<T>();
	treeNode<T>* root = nill;

	treeNode<T>* leftRotate(treeNode<T>* x) {
		treeNode<T>* y = x->right;
		x->right = y->left;

		//int alpha = x->left->descendants + 1, gamma = y->right->descendants + 1;
		//if (x->left == nill) {
		//	alpha--;
		//}
		//if (y->right == nill) {
		//	gamma--;
		//}
		//
		//x->descendants -= (gamma + 1);
		//y->descendants += (alpha + 1);

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

		return y;
	}

	treeNode<T>* rightRotate(treeNode<T>* y) {
		treeNode<T>* x = y->left;
		y->left = x->right;

		//int alpha = x->left->descendants + 1, gamma = y->right->descendants + 1;
		//if (x->left == nill) {
		//	alpha--;
		//}
		//if (y->right == nill) {
		//	gamma--;
		//}
		//
		//x->descendants += (gamma + 1);
		//y->descendants -= (alpha + 1);

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

		return x;
	}

	treeNode<T>* treeMinimum(treeNode<T>* x) {
		while (x->left != nill) {
			x = x->left;
		}
		return x;
	}

	treeNode<T>* treeMaximum(treeNode<T>* x) {
		while (x->right != nill) {
			x = x->right;
		}
		return x;
	}

	treeNode<T>* treeSuccessor(treeNode<T>* x) {
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

	treeNode<T>* treeSearch(treeNode<T>* x, const T& key) {
		if (x == nill || key == x->key) {
			return x;
		}
		if (key < x->key)
			return treeSearch(x->left, key);
		else
			return treeSearch(x->right, key);
	}

	void inorderTreeWalk(treeNode<T>* x, int depth = 0) {
		if (x != nill) {
			inorderTreeWalk(x->left, depth + 1);
			std::cout << x->key << ' ';
			inorderTreeWalk(x->right, depth + 1);
		}
	}

	void zig(treeNode<T>* x) {
		return rightRotate(x->parent);
	}

	void zag(treeNode<T>* x) {
		return leftRotate(x->parent);
	}

	void zigZig(treeNode<T>* x) {
		zig(x->parent);
		zig(x);
	}

	void zagZag(treeNode<T>* x) {
		zag(x->parent);
		zag(x);
	}

	void zigZag(treeNode<T>* x) {
		zag(x);
		zig(x);
	}

	void zagZig(treeNode<T>* x) {
		zig(x);
		zag(x);
	}

	void splay(treeNode<T>* x)
	{
		while (x->parent != NULL)
		{
			treeNode<T>* p = x->parent;
			treeNode<T>* g = p->parent;
			if (g == NULL) zig(x);
			else if (g->left == p && p->left == x) zigZig(x);
			else if (g->right == p && p->right == x) zigZig(x);
			else zigZag(x);
		}
		this->root = x;
	}

public:
	SplayTree() {
		root->parent = nill;
	}

	SplayTree(treeNode<T>* x) {
		root = x;
		root->parent = nill;
	}

	treeNode<T>* newNode(T key) {
		treeNode<T>* newNode = new treeNode<T>();
		newNode->key = key;
		newNode->left = newNode->right = NULL;
		return newNode;
	}

	SplayTree(const std::initializer_list<T>& list) {
		for (const auto& item : list)
			insert(item);
	}

	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, SplayTree<U>& tree) {
		tree.inorderTreeWalk(tree.root);
		return os;
	}

	treeNode<T>* treeSearch(const T& key) {
		treeNode<T>* toFind = treeSearch(root, key);
		return splay(toFind);
	}

	void insert(const T& x)
	{
		if (root == nill)
		{
			root = new treeNode<T>(x);
			return;
		}
		treeNode<T>* curr = this->root;
		while (curr != nill)
		{
			if (x < curr->key)
			{
				if (curr->left == nill)
				{
					treeNode<T>* newNode = new treeNode<T>(x);
					curr->left = newNode;
					newNode->parent = curr;
					splay(newNode);
					return;
				}
				else curr = curr->left;
			}
			else if (x > curr->key)
			{
				if (curr->right == nill)
				{
					treeNode<T>* newNode = new treeNode<T>(x);
					curr->right = newNode;
					newNode->parent = curr;
					splay(newNode);
					return;
				}
				else curr = curr->right;
			}
			else
			{
				splay(curr);
				return;
			}
		}
	}

	void remove(const T& x)
	{
		treeNode<T>* toDelete = treeSearch(x);
		splay(toDelete);

		root = toDelete->left;
		merge(toDelete->right);
	}

	void merge(SplayTree<T>* anotherOne) {
		if (this->treeMaximum > anotherOne->treeMinimum) {
			return;
		}
		splay(this->treeMaximum(this->root));
		this->root->right = anotherOne->root;
	}

	SplayTree<T>* split(treeNode<T>* x) {
		treeNode<T>* xSuccessor = treeSuccessor(x);
		if (xSuccessor == nill) {
			xSuccessor = x;
		}

		splay(x);

		return SplayTree(root->left);
	}
};

int main() {
	SplayTree<WorldMap> tree;

	tree.insert({ "Italy","Rome" });
	std::cout << tree << std::endl;
	tree.insert({ "Spain","Madrid" });
	std::cout << tree << std::endl;
	tree.insert({ "Ukraine","Kyiv" });
	std::cout << tree << std::endl;
	tree.insert({ "Spain","Barcelona" });
	std::cout << tree << std::endl;
	tree.insert({ "Russia","Moscow" });
	std::cout << tree << std::endl;
	tree.insert({ "Italy","Neapol" });

	std::cout << tree << std::endl;

	std::cout << std::endl;

	tree.remove({ "Ukraine","Kyiv" });

	std::cout << tree << std::endl;
	tree.insert({ "USA","New-York" });

	return 0;
}