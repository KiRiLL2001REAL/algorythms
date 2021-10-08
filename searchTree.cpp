#include <iostream>
#include <cassert>
#include <string>
#include <climits>
using namespace std;

class node;
typedef node* node_p;

class node
{
public:
	node(int key): parent(nullptr), right(nullptr), left(nullptr), key(key), counter(1) {};

	inline node_p setRight(node_p x) {
        auto old = right;
	    right = x;
	    if (x != nullptr)
		    x->parent = this;
	    return old;
    }

	inline node_p setLeft(node_p x) {
        auto old = left;
	    left = x;
	    if (x != nullptr)
		    x->parent = this;
	    return old;
    }

	inline node_p changeChild(node_p old, node_p newch) {
        if (left == old)
		    return setLeft(newch);
	    assert (right == old);
	    return setRight(newch);
    }
	
	inline void addChild(int key) {
        if (this->key < key)
		    setRight(new node(key));
	    else
		    setLeft(new node(key));
    }

	node_p parent;
	node_p right;
	node_p left;
	int key, counter;
};

class tree
{
private:
	node_p pseudo;

	void drop(node_p cur) {
        if (cur == nullptr)
		    return;
	    if (cur->left != nullptr) 
		    drop(cur->left);
	    if (cur->right != nullptr)
		    drop(cur->right);
	    delete cur;
    }

    void _dump(node_p node, const string& caseRight = "", const string& caseMiddle = "", const string& caseLeft = "") {
        static string bottomCornerSymb = "L.";
        static string upperConterSymb = "Г`";
        static string middleSymb = "| ";
        if (!node)
            return;
        if (node->right)
            _dump(node->right, caseRight + "  ", caseRight + upperConterSymb, caseRight + middleSymb);
        cout << caseMiddle << node->key << endl;
        if (node->left)
            _dump(node->left, caseLeft + middleSymb, caseLeft + bottomCornerSymb, caseLeft + "  ");
    }

public:
	tree(): pseudo(new node(numeric_limits<int>::max())) {};
	~tree() { drop(pseudo); }

	inline node_p search(int key) {
        auto cur = pseudo->left;
	    while(cur != nullptr) {
		    if (key == cur->key)
			    return cur;
		    cur = key > cur->key ? cur->right : cur->left;
	    }
	    return nullptr;
    }

	inline node_p add(int key) {
        auto pred = pseudo;
	    auto cur = pseudo->left;
	    while (cur != nullptr) {
		    if (cur->key == key) {
			    cur->counter++;
                return cur;
		    }
		    pred = cur;
		    cur = (cur->key < key) ? cur->right : cur->left;
	    }
	    pred->addChild(key);
	    return pred;
    }

	inline node_p minSubnode(node_p cur) {
        while (cur->left != nullptr)
		    cur = cur->left;
	    return cur;
    }

	inline node_p maxSubnode(node_p cur) {
        while (cur->right != nullptr)
		    cur = cur->right;
	    return cur;
    }

	inline node_p succ(node_p node) {
        if (node->right != nullptr)
		    return minSubnode(node->right);
	    auto cur = node->parent;
	    while (cur->key <= node->key)
		    cur = cur->parent;
	    return cur;
    }

	inline node_p extract(node_p x) {
        if (x->right == nullptr)
		    return x->parent->changeChild(x, x->left);
	    else {
		    if (x->left == nullptr)
			    return x->parent->changeChild(x, x->right);
		    else {
			    auto cur = minSubnode(x->right);
			    swap(x->key, cur->key);
			    return extract(cur);
		    }
	    }
    }

	inline void dump() { _dump(pseudo->left); }
};

#include <chrono>

int main()
{
    srand(time(0));

    setlocale(LC_ALL, "en_US.UTF-8");

    tree Tree = tree();
    int in, key;
    cout << "Введите кол-во элементов, которые будут вставлены в дерево: ";
    cin >> in;

    for (int i = 0; i < in; i++) {
        key = (rand() * rand() + rand()) % (2 * in + 1) - in;
        Tree.add(key);
    }

    //cout << "Трассировка дерева:" << endl;
    //Tree.dump();

    cout << endl << "После нажатия Enter будет выполнен тест на скорость поиска. Ключ в диапазоне [" << -in <<  ".." << in << "]" << endl;
    cin.ignore(32768, '\n');
    cin.get();

    auto begin = chrono::steady_clock::now();
    for (int i = -in; i <= in; i++) {
        Tree.search(i);
    }
    auto end = chrono::steady_clock::now();
    auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
    cout << "Поиск выполнен. Прошло " << elapsed_ms.count() << " миллисекунд." << endl;

    cin.get();
    return 0;
}