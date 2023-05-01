# AVL strom
Datová struktura vycházející z binárního vyhledávacího stromu  
Pomocí rotací při vkládání nebo odstraňování vrcholů stromu udržuje vyváženost, tedy že žádný podstrom nemá hloubku svého levého a pravého podstromu vzájemně různou o více jak 1  
Díky tomu je zajištěno, že časová náročnost vyhledávání vrcholu podle hodnoty zůstane vždy log<sub>2</sub>(n)  

## Funkce a metody třídy avl_tree
    bool empty(); // vymaže všechna data

    void remove_all(node* node_); // vymaže všechna data z podstromu vrcholu node_

	void insert(const T& data); // vloží data

    void write() const; // vypíše na řádek vzestupně seřazená data

	void write_balance() const; // vypíše na řádek

	void remove_node(const T& data); // smaže vrchol stromu s danou hodnotou

	void treeprint(); // vykreslí strukturovaně celý strom

	node* find(const T& data, node*& parent); // nalezne vrchol s danou hodnotou
  
  ## Metody třídy avl_tree::node
  
    T getData() const; // vrátí hodnotu vrcholu

	int getBalance() const; // vrátí balanční faktor vrcholu
