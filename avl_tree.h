#pragma once

using T = int;

class avl_tree 
{    
private:
    	class node;

public:
    	avl_tree();

	bool empty();

    	void remove_all(node* node_);

	void insert(const T& data);

    	void write() const;

	void write_balance() const;

	void remove_node(const T& data);

	void treeprint();

	node* find(const T& data, node*& parent);

private:
    	node* root;
    	unsigned int number_of_elements;
	unsigned int number_of_levels;

    	int getDepth(node* node_);

	void remove_leaf(node* to_delete, node* parent);

	void remove_with_one_child(node* to_delete, node* parent);

	void remove_with_two_children(node* to_delete, node* parent);

	void balance_insert(node* end_node);
	
	bool balance_del(node* end_node);

	void write_(node* node_) const;

	void write_balance_(node* end_node) const;

	void right_rotation(node* node_, node* parent);

	void left_rotation(node* node_, node* parent);

	void left_right_rotation(node* node_, node* parent);

	void right_left_rotation(node* node_, node* parent);
};

class avl_tree::node
{

public:
	node(const T& data);

	T getData() const;

	int getBalance() const;

private:
	T data;
	node* left;
	node* right;
    	int balance;
	unsigned int depth;

	friend class avl_tree;
};
