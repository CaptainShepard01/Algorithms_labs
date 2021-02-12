#include <iostream>
#include <exception>
#include <string>


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
	os << '(';

	if (v.city != "") {
		//os << " Country: " << v.country << ", City: " << v.city;
		os << v.city;
	}

	os << " )";
	return os;
}


template <typename T>
struct treeNode {
	T key;

	treeNode* parent;
	treeNode* left = nullptr;
	treeNode* right = nullptr;

	treeNode(T key)
	{
		this->key = key;
		this->parent = NULL;
		this->left = NULL;
		this->right = NULL;
	}
};

template <typename T>
class SplayTree
{
	treeNode<T>* root;
	void print2DUtil(treeNode<T>* root, int space);
	void zig(treeNode<T>*);
	void zig_zig(treeNode<T>*);
	void zig_zag(treeNode<T>*);

	treeNode<T>* treeSuccessor(treeNode<T>* x) {
		if (x->right != NULL) {
			return subtree_min(x->right);
		}
		treeNode<T>* y = x->parent;
		while (y != NULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}
	void splay(treeNode<T>*);

	void getGraphInfo(treeNode<T>* x, std::string& text, int counter) {
		if (x != NULL) {
			std::string counterString = std::to_string(counter);
			if (x->left != NULL)
				text += "\"" + x->key.city + "\"" + " -> " + "\"" + x->left->key.city + "\"" + ";\n";
			if (x->right != NULL)
				text += "\"" + x->key.city + "\"" + " -> " + "\"" + x->right->key.city + "\"" + ";\n";
			getGraphInfo(x->left, text, counter + 1);
			getGraphInfo(x->right, text, counter + 2);
		}
	}

public:
	void print2D();
	SplayTree();
	SplayTree(treeNode<T>*);
	treeNode<T>* find(T);
	treeNode<T>* find(std::string x);

	SplayTree<T> split(treeNode<T>* x);
	void merge(SplayTree<T>* anotherOne);

	void insert(T);
	void Delete(T);
	void inOrderPrint();

	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, SplayTree<U>& tree) {
		tree.inOrderPrint();
		return os;
	}

	std::string getWebGraphviz(std::string graphName = "G") {
		std::string graphText = "digraph " + graphName + " {\n";
		int n = 0;
		getGraphInfo(root, graphText, n);
		return graphText += "}";
	}
};

template <typename T>
void SplayTree<T>::zig(treeNode<T>* x)
{
	treeNode<T>* p = x->parent;
	if (p->left == x)
	{
		treeNode<T>* A = x->left;
		treeNode<T>* B = x->right;
		treeNode<T>* C = p->right;

		x->parent = NULL;
		x->right = p;

		p->parent = x;
		p->left = B;

		if (B != NULL) B->parent = p;
	}
	else
	{
		treeNode<T>* A = p->left;
		treeNode<T>* B = x->left;
		treeNode<T>* C = x->right;

		x->parent = NULL;
		x->left = p;

		p->parent = x;
		p->right = B;

		if (B != NULL) B->parent = p;
	}
}

template <typename T>
void SplayTree<T>::zig_zig(treeNode<T>* x)
{
	treeNode<T>* p = x->parent;
	treeNode<T>* g = p->parent;
	if (p->left == x)
	{
		treeNode<T>* A = x->left;
		treeNode<T>* B = x->right;
		treeNode<T>* C = p->right;
		treeNode<T>* D = g->right;

		x->parent = g->parent;
		x->right = p;

		p->parent = x;
		p->left = B;
		p->right = g;

		g->parent = p;
		g->left = C;


		if (x->parent != NULL)
		{
			if (x->parent->left == g) x->parent->left = x;
			else x->parent->right = x;
		}

		if (B != NULL) B->parent = p;

		if (C != NULL) C->parent = g;
	}
	else
	{
		treeNode<T>* A = g->left;
		treeNode<T>* B = p->left;
		treeNode<T>* C = x->left;
		treeNode<T>* D = x->right;

		x->parent = g->parent;
		x->left = p;

		p->parent = x;
		p->left = g;
		p->right = C;

		g->parent = p;
		g->right = B;

		if (x->parent != NULL)
		{
			if (x->parent->left == g) x->parent->left = x;
			else x->parent->right = x;
		}

		if (B != NULL) B->parent = g;

		if (C != NULL) C->parent = p;
	}
}

template <typename T>
void SplayTree<T>::zig_zag(treeNode<T>* x)
{
	treeNode<T>* p = x->parent;
	treeNode<T>* g = p->parent;
	if (p->right == x)
	{
		treeNode<T>* A = p->left;
		treeNode<T>* B = x->left;
		treeNode<T>* C = x->right;
		treeNode<T>* D = g->right;

		x->parent = g->parent;
		x->left = p;
		x->right = g;

		p->parent = x;
		p->right = B;

		g->parent = x;
		g->left = C;

		if (x->parent != NULL)
		{
			if (x->parent->left == g) x->parent->left = x;
			else x->parent->right = x;
		}

		if (B != NULL) B->parent = p;

		if (C != NULL) C->parent = g;
	}
	else
	{
		treeNode<T>* A = g->left;
		treeNode<T>* B = x->left;
		treeNode<T>* C = x->right;
		treeNode<T>* D = p->right;

		x->parent = g->parent;
		x->left = g;
		x->right = p;

		p->parent = x;
		p->left = C;

		g->parent = x;
		g->right = B;

		if (x->parent != NULL)
		{
			if (x->parent->left == g) x->parent->left = x;
			else x->parent->right = x;
		}

		if (B != NULL) B->parent = g;

		if (C != NULL) C->parent = p;
	}
}

template <typename T>
void SplayTree<T>::splay(treeNode<T>* x)
{
	while (x->parent != NULL)
	{
		treeNode<T>* p = x->parent;
		treeNode<T>* g = p->parent;
		if (g == NULL) zig(x);
		else if (g->left == p && p->left == x) zig_zig(x);
		else if (g->right == p && p->right == x) zig_zig(x);
		else zig_zag(x);
	}
	this->root = x;
}

template <typename T>
SplayTree<T>::SplayTree()
{
	this->root = NULL;
}

template <typename T>
SplayTree<T>::SplayTree(treeNode<T>* rt)
{
	this->root = rt;
}

template <typename T>
treeNode<T>* SplayTree<T>::find(std::string x) {
	T inputData;
	inputData.city = x;

	return this->find(inputData);
}

template <typename T>
treeNode<T>* SplayTree<T>::find(T x)
{
	treeNode<T>* ret = NULL;
	treeNode<T>* curr = this->root;
	treeNode<T>* prev = NULL;
	while (curr != NULL)
	{
		prev = curr;
		if (x < curr->key) curr = curr->left;
		else if (x > curr->key) curr = curr->right;
		else
		{
			ret = curr;
			break;
		}
	}
	if (ret != NULL) splay(ret);
	else if (prev != NULL) splay(prev);
	return ret;
}

template <typename T>
void SplayTree<T>::insert(T x)
{
	if (root == NULL)
	{
		root = new treeNode<T>(x);
		return;
	}
	treeNode<T>* curr = this->root;
	while (curr != NULL)
	{
		if (x < curr->key)
		{
			if (curr->left == NULL)
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
			if (curr->right == NULL)
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

template <typename T>
treeNode<T>* subtree_max(treeNode<T>* subRoot)
{
	treeNode<T>* curr = subRoot;
	while (curr->right != NULL) curr = curr->right;
	return curr;
}

template <typename T>
treeNode<T>* subtree_min(treeNode<T>* subRoot)
{
	treeNode<T>* curr = subRoot;
	while (curr->left != NULL) curr = curr->left;
	return curr;
}

template <typename T>
void SplayTree<T>::Delete(T x)
{
	treeNode<T>* del = find(x);
	if (del == NULL)
	{
		return;
	}
	treeNode<T>* L = del->left;
	treeNode<T>* R = del->right;
	if (L == NULL && R == NULL)
	{
		this->root = NULL;
	}
	else if (L == NULL)
	{
		treeNode<T>* M = subtree_min(R);
		splay(M);
	}
	else if (R == NULL)
	{
		treeNode<T>* M = subtree_max(L);
		splay(M);
	}
	else
	{
		treeNode<T>* M = subtree_max(L);
		splay(M);
		M->right = R;
		R->parent = M;
	}
	delete del;
}

template <typename T>
void printTree(treeNode<T>* x, int depth = 0) {
	if (x != NULL) {
		printTree(x->left, depth + 1);

		if (x->parent != NULL) {
			std::cout << (x->parent->left == x ? "Left " : "Right ") << "son of " << x->parent->key << "_______";
		}
		std::cout << x->key << ";  nodeDepth = " << depth << std::endl;

		printTree(x->right, depth + 1);
	}
}

template <typename T>
void SplayTree<T>::inOrderPrint()
{
	printTree(this->root);
}

/*
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
			std::cout << x->key << ";  nodeDepth = " << depth << std::endl;
			inorderTreeWalk(x->right, depth + 1);
		}
	}

	void zig(treeNode<T>* x) {
		rightRotate(x->parent);
	}

	void zag(treeNode<T>* x) {
		leftRotate(x->parent);
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

	treeNode<T>* splay(treeNode<T>* x)
	{
		while (x->parent != nill)
		{
			treeNode<T>* p = x->parent;
			treeNode<T>* g = p->parent;
			if (g == nill) zig(x);
			else if (g->left == p && p->left == x) zigZig(x);
			else if (g->right == p && p->right == x) zigZig(x);
			else zigZag(x);
		}
		this->root = x;

		return this->root;
	}

public:
	SplayTree() {
		root->parent = nill;
	}

	SplayTree(treeNode<T>* x) {
		root = x;
		root->parent = nill;
	}

	SplayTree(const T& x) {
		root = createNode(x);
	}

	treeNode<T>* createNode(T key) {
		treeNode<T>* newNode = new treeNode<T>();
		newNode->key = key;
		//newNode->parent = nill;
		//
		//newNode->right = nill;
		//newNode->left = nill;
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
			root = createNode(x);
			root->left = nill;
			root->right = nill;
			return;
		}
		treeNode<T>* curr = this->root;
		while (curr != nill)
		{
			if (x < curr->key)
			{
				if (curr->left == nill)
				{
					treeNode<T>* newNode = createNode(x);
					curr->left = newNode;
					newNode->left = nill;
					newNode->right = nill;
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
					treeNode<T>* newNode = createNode(x);
					curr->right = newNode;
					newNode->left = nill;
					newNode->right = nill;
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

		SplayTree<T>* rightTree = new SplayTree<T>(toDelete->right);

		merge(rightTree);
	}

	void merge(SplayTree<T>* anotherOne) {
		if (this->treeMaximum(root)->key > anotherOne->treeMinimum(anotherOne->root)->key) {
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

		SplayTree<T>* splitTree = SplayTree<T>(root->left);

		return splitTree;
	}
};
*/

template <typename T>
void SplayTree<T>::print2DUtil(treeNode<T>* root, int space)
{
	// Base case  
	if (root == NULL)
		return;

	// Increase distance between levels  
	space += 15;

	// Process right child first  
	print2DUtil(root->right, space);

	// Print current node after space  
	// count  
	std::cout << std::endl;
	for (int i = 15; i < space; i++)
		std::cout << " ";
	std::cout << root->key << "\n";

	// Process left child  
	print2DUtil(root->left, space);
}

template <typename T>
void SplayTree<T>::print2D()
{
	// Pass initial space count as 0  
	this->print2DUtil(root, 0);
}


//TO BE IMPLEMENTED
template <typename T>
SplayTree<T> SplayTree<T>::split(treeNode<T>* x) {
	treeNode<T>* xSuccessor = treeSuccessor(x);
	if (xSuccessor == NULL) {
		xSuccessor = x;
	}

	splay(x);

	//root->left->parent = NULL;
	//root->right->parent = NULL;
	SplayTree<T> splitTree = SplayTree<T>(root->left);

	return splitTree;
}

//TO BE IMPLEMENTED
template <typename T>
void SplayTree<T>::merge(SplayTree<T>* anotherOne) {
	if (this->treeMaximum(root)->key > anotherOne->treeMinimum(anotherOne->root)->key) {
		return;
	}
	splay(this->treeMaximum(this->root));
	this->root->right = anotherOne->root;
}

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
	std::cout << tree.getWebGraphviz() << std::endl;

	tree.insert({ "Russia","Moscow" });
	std::cout << tree << std::endl;
	std::cout << tree.getWebGraphviz() << std::endl;

	tree.insert({ "Italy","Neapol" });
	std::cout << tree << std::endl;
	std::cout << tree.getWebGraphviz() << std::endl;


	tree.Delete({ "Ukraine","Kyiv" });
	std::cout << tree << std::endl;
	std::cout << tree.getWebGraphviz() << std::endl;

	tree.insert({ "USA","New-York" });
	std::cout << tree << std::endl;

	std::cout << std::endl << std::endl;
	tree.print2D();
	std::cout << std::endl << std::endl;


	std::cout << tree.getWebGraphviz() << std::endl;

	//treeNode<WorldMap>* toSplit = tree.find("Neapol");
	//SplayTree<WorldMap> leftTree = tree.split(toSplit);
	//
	//std::cout << leftTree << std::endl;
	//std::cout << tree << std::endl;

	return 0;
}