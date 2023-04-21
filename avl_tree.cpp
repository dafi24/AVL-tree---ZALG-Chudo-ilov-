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

int avl_tree::node::getBalance() const {
	return this->balance;
}

int avl_tree::getDepth(node* node_)
{
	if (node_ != nullptr)
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

				this->number_of_levels = (help_depth + 1) > this->number_of_levels ? (help_depth + 1) : this->number_of_levels;
				balance(help->right);
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

				this->number_of_levels = (help_depth + 1) > this->number_of_levels ? (help_depth + 1) : this->number_of_levels;
				balance(help->left);
				return;
			}
			help = help->left;
			help_depth++;
		}
	}
}

void avl_tree::treeprint()
{
	std::cout << std::endl << std::endl;

	struct node_help
	{
		node* p;
		bool is_separator;
	};

	std::queue<node_help> q;

	q.push(node_help{ root, false });
	q.push(node_help{ nullptr, true });

	int help_depth = 0;

	int space = pow(2, this->number_of_levels - root->depth + 2);
	std::cout << std::setw(space) << "";

	while (!q.empty())
	{
		node_help help = q.front();
		q.pop();

		if (help.p != nullptr)
		{
			q.push(node_help{ help.p->left, false });
			q.push(node_help{ help.p->right, false });

			int result = ((help.p->data <= 1) ? 1 : log10(help.p->data) + 1);

			std::cout << help.p->data;

			space = pow(2, this->number_of_levels - help.p->depth + 2);
			std::cout << std::setw(2 * space - result) << "";
		}
		else if (help.is_separator == true)
		{
			help_depth++;

			if (q.empty())
			{
				break;
			}

			std::cout << std::endl;
			space = pow(2, this->number_of_levels - help_depth + 2);
			std::cout << std::setw(space) << "";
			q.push(node_help{ nullptr, true });

			if (help_depth > this->number_of_levels)
			{
				break;
			}
		}
		else
		{
			q.push(node_help{ nullptr, false });
			q.push(node_help{ nullptr, false });
			std::cout << "x";
			space = pow(2, this->number_of_levels - help_depth + 2);

			std::cout << std::setw(2 * space - 1) << "";
		}
	}

	std::cout << std::endl << std::endl;
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

	while (help != nullptr) {
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

void avl_tree::remove_node(const T& data)
{
	node* parent = nullptr;
	node* to_delete = find(data, parent);

	if (to_delete == nullptr)
	{
		return;
	}

	if (to_delete->left && to_delete->right)
	{
		remove_with_two_children(to_delete, parent);
		balance(parent);
	}
	else if (to_delete->left || to_delete->right)
	{
		remove_with_one_child(to_delete, parent);
		balance(parent);
	}
	else
	{
		remove_leaf(to_delete, parent);
		balance(parent);
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
	parent = to_delete;

	while (help->right) {
		parent = help;
		help = help->right;
	}

	to_delete->data = help->getData();

	if (help->left != nullptr) {
		remove_with_one_child(help, parent);
	} else {
		remove_leaf(help, parent);
	}
}

void avl_tree::balance(node* end_node) 
{
	if (end_node == nullptr) return;
	
	node* parent = nullptr;
	node* grandparent = nullptr;
	node* help = root;

	while (help != nullptr) 
	{
		if (help->getData() == end_node->data) return;
		
		help->balance = getDepth(help->left) - getDepth(help->right);
		
		if (help->balance < -1) 
		{
			if (help == root) left_rotation(help, nullptr);
			else if (parent->left == help) left_right_rotation(parent, grandparent);
			else if (parent->right == help) left_rotation(parent, grandparent);
			recalculate_after_balance(parent);
			return;
		} 
		else if (help->balance > 1) 
		{
			if (help == root) right_rotation(help, nullptr);
			else if (parent->left == help) right_rotation(parent, grandparent);
			else if (parent->right == help) right_left_rotation(parent, grandparent);
			recalculate_after_balance(parent);
			return;
			
		}

		grandparent = parent;
		parent = help;

		if (end_node->data > help->getData()) 
		{
			if (help->right == nullptr) return;
			help = help->right;
		}
		else 
		{
			if (help->left == nullptr) return;
			help = help->left;
		}
	}
}

void avl_tree::write() const
{
	write_(root);
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

void avl_tree::recalculate_after_balance(node* node_)
{	
	recalculate_after_balance_(node_);

	int left_depth = getDepth(root->left) + 1;
	int right_depth = getDepth(root->right) + 1;
	number_of_levels = left_depth > right_depth ? left_depth : right_depth;
}

void avl_tree::recalculate_after_balance_(node* node_)
{
	if (node_ == nullptr) return;

	recalculate_after_balance(node_->left);
	recalculate_after_balance(node_->right);

	int left_depth = getDepth(node_->left) + 1;
	int right_depth = getDepth(node_->right) + 1;

	node_->balance = left_depth - right_depth;
	node_->depth = left_depth > right_depth ? left_depth : right_depth;
}