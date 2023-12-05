//
// Created by Матвей Галимьянов on 01.12.2023.
//

#include "Inventory.h"

Inventory::Inventory(size_t x, size_t y) : inventory(x, y) {
    for (auto const i : inventory) {
        for (int j = 0; j < inventory.size().second; j++) {
            i[j] = nullptr;
        }
    }
}

Matrix<Item*> & Inventory::data() { return inventory; }

Item* Inventory::find(std::string const& name) {
    for (auto const i : inventory) {
        for (int j = 0; j < inventory.size().second; j++) {
            if(i[j]->getName() == name) {
                return i[j];
            }
        }
    }
    throw std::runtime_error("\"" + name + "\" not found");
}

Item* Inventory::find(Item* item) {
    if(!item) {
        throw std::logic_error("nullptr pointer for item");
    }
    for (auto const i : inventory) {
        for (int j = 0; j < inventory.size().second; j++) {
            if(i[j] == item) {
                return i[j];
            }
        }
    }
    throw std::runtime_error("\"" + item->getName() + "\" not found");
}


bool Inventory::isEmpty() {
    return (inventory.size().first == 0);
}

bool Inventory::addItemToInventory(Item *item) {
    for (auto const i : inventory) {
        for (int j = 0; j < inventory.size().second; j++) {
            if(i[j] == nullptr) {
                i[j] = item;
                return true;
            }
        }
    }
    return false;
}

Item* Inventory::changeItem(Item *item, size_t x, size_t y) {
    if(x >= inventory.size().first || y >= inventory.size().second) {
        throw std::logic_error("out of range matrix");
    }
    Item* swap = inventory[x][y];
    inventory[x][y] = item;
    return swap;
}

Item* Inventory::changeItem(Item *item, std::string const& name) {
    for (auto const i : inventory) {
        for (int j = 0; j < inventory.size().second; j++) {
            if(i[j]->getName() == name) {
                Item *swap = i[j];
                i[j] = item;
                return swap;
            }
        }
    }
    throw std::runtime_error("\"" + name + "\" not found");
}
//Переделать
std::vector<Item*> Inventory::throwAllItems() {
    std::vector<Item*> throwItems(0);
    for (int i = 0; i < inventory.size().first; ++i) {
        for (int j = 0; j < inventory.size().second; ++j) {

        }
    }
    return throwItems;
}

Inventory::~Inventory() {
    for (auto const i : inventory) {
        for (int j = 0; j < inventory.size().second; j++) {
            delete i[j];
        }
    }
}