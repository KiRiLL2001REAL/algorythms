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
	
	inline node_p addChild(int key) {
        if (this->key < key) {
		    setRight(new node(key));
            return right;
        }
		setLeft(new node(key));
        return left;
    }

	node_p parent;
	node_p right;
	node_p left;
	int key, counter;
};

class alphaTree
{
private:
	node_p pseudo;
    int count;
    float alpha;

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

    int _size(node_p cur) {
        if (cur == nullptr)
            return 0;
        return _size(cur->left) + _size(cur->right) + 1;
    }

    node_p _findScapegoat(node_p cur) {
        if (cur == pseudo)
            return nullptr;
        auto value = alpha * _size(cur);
        if (value >= _size(cur->left) || value >= _size(cur->right))
            return cur;
        return _findScapegoat(cur->parent);
    }

    реализовать сброс неправильной ветки в массив

    реализовать пересборку неправильной ветки из массива

public:
	alphaTree(float alpha): pseudo(new node(numeric_limits<int>::max())), count(0), alpha(alpha) {};
	~alphaTree() { drop(pseudo); }

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
	    auto scapeGoat = _findScapegoat(pred->addChild(key));
        
        //_incWeightUp(pred);==========================================================================================================================
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
        node_p answ;
        if (x->right == nullptr)
		    answ = x->parent->changeChild(x, x->left);
	    else {
		    if (x->left == nullptr)
			    answ = x->parent->changeChild(x, x->right);
		    else {
			    auto cur = minSubnode(x->right);
			    swap(x->key, cur->key);
			    answ =  extract(cur);
		    }
	    }
        //_recalcWeights(pseudo->left);==========================================================================================================================
        return answ;
    }

	inline void dump() { _dump(pseudo->left); }
};

#include <chrono>

int main()
{
    srand(time(0));

    setlocale(LC_ALL, "en_US.UTF-8");

    auto Tree = alphaTree(0.7f);
    
    for (int i = 0; i < 9; i++)
        Tree.add(i);
    Tree.add(-5);
    Tree.add(10);
    Tree.add(9);

    cout << "Трассировка дерева:" << endl;
    Tree.dump();

    cin.get();
    return 0;
}