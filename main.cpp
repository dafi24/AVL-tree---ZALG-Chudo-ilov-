#include <iostream>
#include "avl_tree.h"
#include "avl_tree.cpp"

using namespace std;

int main() {
    avl_tree t;
    
    t.insert(3);
    t.treeprint();

    t.insert(8);
    t.treeprint();

    t.insert(9);
    t.treeprint();

    t.insert(5);
    t.treeprint();

    t.insert(-5);
    t.treeprint();

    t.insert(-1);
    t.treeprint();

    t.insert(1);
    t.treeprint();
    
    t.insert(-3);
    t.treeprint();

    t.insert(2);
    t.treeprint();



    t.remove_node(-1);
    t.treeprint();

    t.remove_node(9);
    t.treeprint();

    t.remove_node(8);
    t.treeprint();

    t.remove_node(3);
    t.treeprint();

    std::cin.get();
    return 0;
}