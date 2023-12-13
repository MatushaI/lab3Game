#include "items.h"

#include <unordered_set>

// {Название}

// {Название, Weapon(Название, вес, урон, время стрельбы, время перезарядки, тип патронов, вместимость "магазина")}
// КАК ОТЛОВИТЬ THROW???

std::unordered_set<std::string> AmmoTypeRepo::names_ = { "12,7mm"};

std::unordered_map<std::string, Weapon> Weapon::baseWeapon = {
    {"MachineGun", Weapon("MachineGun", 25500, 50, 1, 10, "12,7mm", 300)},
    {"AK-47", Weapon("AK-47", 10000, 100, 2, 1, "12,7mm", 1000)}
};
