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
	void Delete(std::string);
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
void SplayTree<T>::Delete(T x) {
	treeNode<T>* del = find(x);
	if (del == NULL)
		return;

	treeNode<T>* L = del->left;
	treeNode<T>* R = del->right;

	if (L == NULL && R == NULL)
		root = NULL;
	else if (L == NULL) {
		treeNode<T>* M = subtree_min(R);
		splay(M);
	}
	else if (R == NULL) {
		treeNode<T>* M = subtree_max(L);
		splay(M);
	}
	else {
		treeNode<T>* M = subtree_max(L);
		splay(M);
		M->right = R;
		R->parent = M;
	}

	//if (del->parent) {
	//	if (del->parent->right == del)
	//		del->parent->right = NULL;
	//	else
	//		del->parent->left = NULL;
	//}

	delete del;
}

template <typename T>
void SplayTree<T>::Delete(std::string x) {
	T inputData;
	inputData.city = x;

	Delete(inputData);
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
	tree.print2D();
	std::cout <<std:: endl;

	tree.insert({ "Spain","Madrid" });
	std::cout << tree << std::endl;
	tree.print2D();
	std::cout << std::endl;

	tree.insert({ "Ukraine","Kyiv" });
	std::cout << tree << std::endl;
	tree.print2D();
	std::cout << std::endl;

	tree.insert({ "Spain","Barcelona" });
	std::cout << tree << std::endl;
	tree.print2D();
	std::cout << std::endl;

	tree.insert({ "Russia","Moscow" });
	std::cout << tree << std::endl;
	tree.print2D();
	std::cout << std::endl;

	tree.insert({ "Italy","Neapol" });
	std::cout << tree << std::endl;
	tree.print2D();
	std::cout << std::endl;

	tree.Delete("Moscow");
	std::cout << tree << std::endl;
	tree.print2D();
	std::cout << std::endl;

	tree.insert({ "USA","New-York" });
	std::cout << tree << std::endl;
	tree.print2D();
	std::cout <<std:: endl;

	std::cout << std::endl << std::endl;
	tree.print2D();
	std::cout << std::endl << std::endl;


	//treeNode<WorldMap>* toSplit = tree.find("Neapol");
	//SplayTree<WorldMap> leftTree = tree.split(toSplit);
	//
	//std::cout << leftTree << std::endl;
	//std::cout << tree << std::endl;

	return 0;
}