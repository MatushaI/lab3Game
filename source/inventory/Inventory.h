#ifndef INVENTORY_H
#define INVENTORY_H

#include "items.h"
#include "matrix_lib.h"

class Inventory{
public:
    Inventory(size_t x, size_t y);

    Matrix<Item*> & data();
    Item* find(std::string const& name);
    Item* find(Item * item);
    bool isEmpty();
    bool addItemToInventory(Item * item);
    Item * changeItem(Item * item, size_t x, size_t y);
    Item * changeItem(Item * item, std::string const& name);
    std::vector<Item*> throwAllItems();

    ~Inventory();
private:
    Matrix<Item*> inventory;
};

#endif //INVENTORY_H
