#include <bits/stdc++.h>
using namespace std;

class node
{
	int key;
	node *left;
	node *right;
	node *parent;
	int balfac;

public:
	int vis()
	{
		return this->key;
	}
	void rotate_left()
	{
		// check for null pointer dereferencing a,b,c can be null
		auto par = this->parent;
		auto y = this;
		auto x = this->right;
		auto a = this->left;
		auto b = x->left;
		auto c = x->right;
		if (par)
		{
			x->parent = par;
			if (par->left == this)
				par->left = x;
			else
				par->right = x;
		}
		x->left = y;
		y->parent = x;
		x->right = c;
		c->parent = x;
		y->left = a;
		a->parent = y;
		y->right = b;
		b->parent = y;
		// y->balfac = b->balfac - a->balfac;
		// x->balfac = c->balfac - y->balfac;
	}
	void rotate_right()
	{
		auto par = this->parent;
		auto x = this;
		auto y = this->left;
		auto a = this->left->left;
		auto b = this->left->right;
		auto c = this->right;
		if (par)
		{
			y->parent = par;
			if (par->left == this)
				par->left = y;
			else
				par->right = y;
		}
		y->right = x;
		x->parent = y;
		x->right = c;
		c->parent = x;
		x->left = b;
		b->parent = x;
		y->left = a;
		a->parent = y;
		// x->balfac = c->balfac - b->balfac;
		// y->balfac = x->balfac - a->balfac;
	}
	void rebalance(bool h_inc)
	{
		if(abs(this->balfac)>=2)
		{
			///
		}
	}
	node(int x)
	{
		this->key = x;
		this->parent = NULL;
		this->left = NULL;
		this->right = NULL;
		this->balfac = 0;
	}
	int search(int x)
	{
		if (this->key == x)
			return 1;
		else if (this->key > x and this->left != NULL)
			return this->left->search(x);
		else if (this->key < x and this->right != NULL)
			return this->right->search(x);
		else
			return 0;
	}
	void insert(int x)
	{
		if (this->key == x)
			cout << "Key already exists\n";
		else if (this->key > x)
		{
			if (this->left != NULL)
				this->left->insert(x);
			else
			{
				this->left = new node(x);
				this->left->parent = this;
				if (this->right)
				{
					this->balfac = 0;
					rebalance(0);
				}
				else
				{
					this->balfac = -1;
					rebalance(1);
				}
			}
		}
		else if (this->key < x)
		{
			if (this->right != NULL)
				this->right->insert(x);
			else
			{
				this->right = new node(x);
				this->right->parent = this;
				if (this->left)
				{
					this->balfac = 0;
					rebalance(0);
				}
				else
				{
					this->balfac = 1;
					rebalance(1);
				}
			}
		}
		else
			assert(0);
	}
	void inorder()
	{
		if (this->left)
			this->left->inorder();
		cout << "key: " << this->key;
		if (this->left)
			cout << "\t left: " << this->left->key;
		else
			cout << "\t no left ";
		if (this->right)
			cout << "\t right: " << this->right->key;
		else
			cout << "\t no right ";
		if (this->parent)
			cout << "\t parent: " << this->parent->key;
		else
			cout << "\t root ";
		// cout << "\t height: " << this->height << endl;
		if (this->right)
			this->right->inorder();
	}
	node *get_leftmost()
	{
		if (this->left)
			return this->left->get_leftmost();
		return this;
	}
	node *get_rightmost()
	{
		if (this->right)
			return this->right->get_rightmost();
		return this;
	}
	node *succ(int x)
	{
		if (this->key == x)
		{
			if (this->right)
			{
				return this->right->get_leftmost();
			}
			else
			{
				auto x1 = this;
				auto y = this->parent;
				while (y != NULL and x1 == y->right)
				{
					x1 = y;
					y = y->parent;
				}
				if (y == NULL or y->key == x)
				{
					cout << "Most probably largest elem ";
					return this;
				}
				return y;
			}
		}
		else if (this->key > x and this->left)
		{
			return this->left->succ(x);
		}
		else if (this->key < x and this->right)
		{
			return this->right->succ(x);
		}
		else
		{
			cout << "Key doesnt exist" << endl;
			return NULL;
		}
	}
	node *pred(int x)
	{
		if (this->key == x)
		{
			if (this->left)
				return this->left->get_rightmost();
			else
			{
				auto x1 = this;
				auto y = x1->parent;
				while (y != NULL and x1 == y->left)
				{
					x1 = y;
					y = x1->parent;
				}
				if (y == NULL or y->key == x)
				{
					cout << "Most probably smolest elem ";
					return this;
				}
				return y;
			}
		}
		else if (this->key > x and this->left)
		{
			return this->left->pred(x);
		}
		else if (this->key < x and this->right)
		{
			return this->right->pred(x);
		}
		else
		{
			cout << "Key doesnt exist" << endl;
			return NULL;
		}
	}
	int del(int x)
	{
		if (this->key == x)
		{
			if (this->left == NULL and this->right == NULL)
			{
				if (this->parent->left == this)
					this->parent->left = NULL;
				else
					this->parent->right = NULL;
				// this->parent->height=1;
				// this->parent->upd_h(1);
			}
			else if (this->left == NULL)
			{
				if (this->parent)
				{
					if (this->parent->left == this)
					{
						this->parent->left = this->right;
					}
					else
					{
						this->parent->right = this->right;
					}
					this->right->parent = this->parent;
					// this->right->height=1;
					// this->right->upd_h(1);
				}
				// delete(this);
			}
			else if (this->right == NULL)
			{
				if (this->parent)
				{
					if (this->parent->left == this)
					{
						this->parent->left = this->left;
					}
					else
					{
						this->parent->right = this->left;
					}
					this->left->parent = this->parent;
					// this->left->height=1;
					// this->left->upd_h(1);
				}
				// delete(this);
			}
			else
			{
				auto suc = this->succ(this->key);
				this->del(suc->key);
				this->key = suc->key;
			}
			return 1;
		}
		else if (this->key > x and this->left)
			return this->left->del(x);
		else if (this->key < x and this->right)
			return this->right->del(x);
		else
			return 0;
	}
	~node()
	{
		if (this->left)
			delete (this->left);
		if (this->right)
			delete (this->right);
	}
};
int main()
{
	node root(1);
	for (int i = 0; i < 6; i++)
	{
		int x;
		cin >> x;
		root.insert(x);
	}
	root.inorder();

	for (int i = 0; i < 7; i++)
	{
		int x;
		cin >> x;
		if (root.search(x))
		{
			cout << "succ: " << root.succ(x)->vis() << endl;
			cout << "pred: " << root.pred(x)->vis() << endl;
		}
		else
		{
			cout << "Node not found" << endl;
		}
	}

	cout << "deleting\n";
	for (int i = 0; i < 7; i++)
	{
		int x;
		cin >> x;
		cout << (root.del(x) ? "Yes" : "No") << endl;
		root.inorder();
		cout << endl;
	}

	return 0;
}