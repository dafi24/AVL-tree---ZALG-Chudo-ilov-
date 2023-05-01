#include "avl_tree.h"
#include <iostream>
#include <cmath>
#include <queue>
#include <iomanip>

avl_tree::avl_tree()
{
	root = nullptr;
	number_of_elements = 0;
	number_of_levels = 0;
}

avl_tree::node::node(const T& data)
	: data(data), left(nullptr), right(nullptr), balance(0), depth(0)
{

}

T avl_tree::node::getData() const
{
	return this->data;
}

int avl_tree::node::getBalance() const 
{
	return this->balance;
}

int avl_tree::getDepth(node* node_)
{
	if (node_)
	{
		int l = 0, p = 0;
		l = getDepth(node_->left);
		p = getDepth(node_->right);
		return l > p ? (l + 1) : (p + 1);
	}
	return 0;

}

bool avl_tree::empty()
{
	return root == nullptr;
}

void avl_tree::insert(const T& data)
{
	if (empty())
	{
		root = new node(data);
		number_of_elements++;
		return;
	}

	node* help = root;

	int help_depth = 0;

	while (true)
	{
		if (data == help->data)
		{
			return;
		}
		else if (data > help->data)
		{
			if (help->right == nullptr)
			{
				help->right = new node(data);
				help->right->depth = help_depth + 1;
				number_of_elements++;

				balance_insert(help->right);
				this->number_of_levels = getDepth(root);
				return;
			}
			help = help->right;
			help_depth++;
		}
		else
		{
			if (help->left == nullptr)
			{
				help->left = new node(data);
				help->left->depth = help_depth + 1;
				number_of_elements++;

				balance_insert(help->left);
				this->number_of_levels = getDepth(root);
				return;
			}
			help = help->left;
			help_depth++;
		}
	}
}

void avl_tree::treeprint()
{
	unsigned int h = getDepth(root); // Vypocti vysku stromu
	unsigned int space = (1 << (h - 1)); // mezera pred a za kazdym prvkem je 2^(h-1)
	std::queue<node*> q; // Fronta na uchovavani jednotlivych urovni stromu
	q.push(root);        // vloz 1. uroven do stromu
	node* curr;          // pomocna promenna ukazujici na aktualni vypisovany prvek

	for (unsigned int i = 1; i <= h; i++) // Projdi vsech h pater
	{
		unsigned int level_size = q.size(); // velikost i-te urovne - (slo by i jinak protoze delame siroky vypis, ale lookup na q.size() je O(1) takze neni potreba rucne pocitat)
		for (unsigned int j = 0; j < level_size; j++) // Projdi vsech k prvku na patre
		{
			// postupne vyjmi kazdy prvek z i-teho patra
			curr = q.front();
			q.pop();
			//
			std::cout << std::setw(space * 2) << std::right; // cislo kterym nasobime space je maximalni pocet cislic klice
			if (curr) { // existuje-li aktualni prvek
				// vloz jeho syny do frontu (do dalsiho patra)
				q.push(curr->left);
				q.push(curr->right);
				// a vypis jeho klic
				std::cout << curr->data;
			}
			else { // neexistuje-li aktualni prvek (prvek je nullptr)
				// vloz do fronty jako jeho syny zase nullptr
				q.push(nullptr);
				q.push(nullptr);
				// Vypis mezeru misto jeho hodnoty
				std::cout << "  "; // lze prepsat na "xx" a bude na miste chybejiciho vrcholu psat "xx"
			}
			// vynech zase mezeru za prvkem
			std::cout << std::setw(space * 2) << " "; // cislo kterym nasobime space je maximalni pocet cislic klice
		}
		space /= 2; // zmensi mezeru na dalsi urovni na polovinu
		std::cout << std::endl; // prejdi na novy radek
	}
	std::cout << std::endl;
}

void avl_tree::remove_all(node* node_)
{
	if (node_ == nullptr)
	{
		return;
	}

	remove_all(node_->left);
	remove_all(node_->right);

	delete node_;
}

avl_tree::node* avl_tree::find(const T& data, node*& parent) {
	parent = nullptr;
	if (empty()) {
		return nullptr;
	}
	node* help = root;

	while (help) {
		if (help->getData() == data) return help;

		parent = help;

		if (data > help->getData()) {
			if (help->right == nullptr) return nullptr;
			help = help->right;
		}
		else {
			if (help->left == nullptr) return nullptr;
			help = help->left;
		}
	}
	return nullptr;
}

using namespace std;

void avl_tree::remove_node(const T& data)
{
	node* parent = nullptr;
	node* to_delete = find(data, parent);

	if (to_delete == nullptr) return;

	bool leftChild;

	if (to_delete != root) leftChild = to_delete == parent->left ? true : false;
	
	if (to_delete->left && to_delete->right)
	{
		remove_with_two_children(to_delete, parent);
	}
	else if (to_delete->left || to_delete->right)
	{
		remove_with_one_child(to_delete, parent);
		
		if (leftChild)
		{
			if (!balance_del(parent->left)) balance_insert(parent->left);
		}
		else 
		{			
			if (!balance_del(parent->right)) balance_insert(parent->right);
		}
	}
	else
	{
		remove_leaf(to_delete, parent);
		if (!balance_del(parent)) balance_insert(parent);
	}
}

void avl_tree::remove_leaf(node* to_delete, node* parent)
{
	if (parent)
	{
		if (parent->left == to_delete)
		{
			parent->left = nullptr;
		}
		else
		{
			parent->right = nullptr;
		}
	}
	else
	{
		root = nullptr;
	}

	delete to_delete;
	number_of_elements--;

	this->number_of_levels = getDepth(root);
}

void avl_tree::remove_with_one_child(node* to_delete, node* parent)
{
	if (parent)
	{
		if (parent->left == to_delete)
		{
			parent->left = to_delete->left ? to_delete->left : to_delete->right;
		}
		else
		{
			parent->right = to_delete->left ? to_delete->left : to_delete->right;
		}
	}
	else
	{
		root = to_delete->left ? to_delete->left : to_delete->right;
	}

	delete to_delete;
	number_of_elements--;

	this->number_of_levels = getDepth(root);
}

void avl_tree::remove_with_two_children(node* to_delete, node* parent) 
{
	node* help = to_delete->left;
	node* grandparent = parent;
	parent = to_delete;
	 
	while (help->right) 
	{
		grandparent = parent;
		parent = help;
		help = help->right;
	}

	to_delete->data = help->getData();

	if (help->left) {
		remove_with_one_child(help, parent);
		if (!balance_del(help->left)) balance_insert(help->left);
	} else {
		remove_leaf(help, parent);
		if (!balance_del(parent)) balance_insert(parent);
	}
}

void avl_tree::balance_insert(node* end_node) 
{
	if (end_node == nullptr) return;
	
	node* parent = nullptr;
	node* help = root;

	node* unbalanced_min = nullptr;
	node* unbalanced_min_parent = nullptr;

	while (help) 
	{	
		help->balance = getDepth(help->left) - getDepth(help->right);

		if (abs(help->balance) > 1)
		{
			unbalanced_min_parent = parent;
			unbalanced_min = help;
		} 

		if (help->getData() == end_node->getData()) break;

		parent = help;

		if (end_node->getData() > help->getData()) 
		{
			if (help->right == nullptr) break;
			help = help->right;
		}
		else 
		{
			if (help->left == nullptr) break;
			help = help->left;
		}
	}
	
	if (unbalanced_min)
	{
		if (unbalanced_min->balance > 1) 
		{
			if (end_node->getData() < unbalanced_min->left->getData()) right_rotation(unbalanced_min, unbalanced_min_parent);
			else left_right_rotation(unbalanced_min, unbalanced_min_parent);
		}
		else
		{
			if (end_node->getData() > unbalanced_min->right->getData()) left_rotation(unbalanced_min, unbalanced_min_parent);
			else right_left_rotation(unbalanced_min, unbalanced_min_parent);
		}
	}
}

bool avl_tree::balance_del(node* end_node) 
{	
	node* parent = nullptr;
	node* help = root;

	node* unbalanced_min = nullptr;
	node* unbalanced_min_parent = nullptr;
	bool leftChild;

	while (help) 
	{		
		help->balance = getDepth(help->left) - getDepth(help->right);

		if (abs(help->balance) > 1)
		{
			unbalanced_min_parent = parent;
			unbalanced_min = help;
			if (unbalanced_min != root) leftChild = unbalanced_min == unbalanced_min_parent->left ? true : false;
		}
		parent = help;

		if (end_node->getData() > help->getData()) 
		{
			if (help->right == nullptr) break;
			help = help->right;
		}
		else 
		{
			if (help->left == nullptr) break;
			help = help->left;
		}
	}

	if (!unbalanced_min) return false;

	if (unbalanced_min == root)
	{
		if (root->balance > 1)
		{
			if (getDepth(root->left->right) > getDepth(root->left->left)) left_right_rotation(root, nullptr);
			else right_rotation(root, nullptr);
		}
		else
		{
			if (getDepth(root->right->left) > getDepth(root->right->right)) right_left_rotation(root, nullptr);
			else left_rotation(root, nullptr);
		}
	} 
	else
	{
		node* y = getDepth(unbalanced_min->left) > getDepth(unbalanced_min->right) ? unbalanced_min->left : unbalanced_min->right;
		node* z = getDepth(y->left) > getDepth(y->right) ? y->left : y->right;

		if (unbalanced_min->left == y)
		{
			if (y->left == z) right_rotation(unbalanced_min, unbalanced_min_parent);
			else left_right_rotation(unbalanced_min, unbalanced_min_parent);
		}
		else
		{
			if (y->left == z) right_left_rotation(unbalanced_min, unbalanced_min_parent);
			else left_rotation(unbalanced_min, unbalanced_min_parent);
		}
		balance_insert(leftChild ? unbalanced_min_parent->left : unbalanced_min_parent->right);
	}
	return true;
}

void avl_tree::write() const
{
	write_(root);
	std::cout << std::endl;
}

void avl_tree::write_balance() const 
{
	write_balance_(root);
}

void avl_tree::write_(node* node_) const
{
	if (node_ == nullptr) return;
	write_(node_->left);
	std::cout << node_->getData() << " ";
	write_(node_->right);

}

void avl_tree::write_balance_(node* node_) const 
{
	if (node_ == nullptr) return;
	write_balance_(node_->left);
	std::cout << node_->getBalance() << " ";
	write_balance_(node_->right);
}

void avl_tree::right_rotation(node* node_, node* parent) 
{	
	if (node_ == nullptr) return;
	if (node_->left == nullptr) return;

	node* left_child = node_->left;
	node_->left = left_child->right;

	if (node_ == root) 
	{
		root = left_child;
		root->right = node_;
	}
	else if (parent->left == node_) 
	{
		parent->left = left_child;
		parent->left->right = node_;
	}
	else 
	{
		parent->right = left_child;
		parent->right->right = node_;
	}
}

void avl_tree::left_rotation(node* node_, node* parent) 
{
	if (node_ == nullptr) return;
	if (node_->right == nullptr) return;

	node* right_child = node_->right;
	node_->right = right_child->left;

	if (node_ == root) 
	{
		root = right_child;
		root->left = node_;
	} 
	else if (parent->left == node_) 
	{
		parent->left = right_child;
		parent->left->left = node_;
	}
	else 
	{
		parent->right = right_child;
		parent->right->left = node_;
	}
}

void avl_tree::left_right_rotation(node* node_, node* parent) 
{
	if (node_ == nullptr) return;
	if (node_->left == nullptr) return;
	if (node_->left->right == nullptr) return;
	left_rotation(node_->left, node_);
	right_rotation(node_, parent);
}

void avl_tree::right_left_rotation(node* node_, node* parent) 
{
	if (node_ == nullptr) return;
	if (node_->right == nullptr) return;
	if (node_->right->left == nullptr) return;
	right_rotation(node_->right, node_);
	left_rotation(node_, parent);
}
