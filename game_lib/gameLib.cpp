#include "gameLib.hpp"
//#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <regex>
#include <utility>

AmmoTypeRepo::AmmoTypeRepo() {
    names_.insert("");
}

AmmoTypeRepo & AmmoTypeRepo::instance()  {
    static AmmoTypeRepo repo;
    return repo;
};

bool AmmoTypeRepo::find(std::string const&name) {
    return names_.count(name) ? true : false;
}

bool AmmoTypeRepo::insert(std::string const&name) {
    return names_.insert(name).second; // возвращает bool успешная вставка или нет
}

// конструкторы
cartridgeType::cartridgeType() {
    name_ = "";
}

cartridgeType::cartridgeType(std::string const& name) {
    if(!typesRepo.find(name)) {
        throw std::logic_error("\"" + name + "\" isn't a base cartridgeType");
    }
    name_ = name;
}

cartridgeType::cartridgeType(std::vector<std::string> const& names) {
    for (auto & i : names) {
        if(!typesRepo.find(i)) {
            typesRepo.insert(i);
        }
    }
}

void cartridgeType::addCartridgeType(std::string const& name) {
    if(!typesRepo.find(name)) {
        typesRepo.insert(name);
    }
}

// сравнение типов патронов
bool cartridgeType::operator==(cartridgeType const& type) {
    return name_ == type.name_;
}

bool cartridgeType::operator!=(cartridgeType const& type) {
    return name_ != type.name_;
}

// getter
std::string const& cartridgeType::getName() const noexcept{
    return name_;
}

//Item

Item::Item(std::string const& nameItem, size_t weight) : nameItem_(nameItem), weight_(weight) {}

std::string const& Item::getName() { return nameItem_; }
size_t Item::getWeight() { return weight_; }

void Item::setName(std::string const& name) {
    if(name.size() == 0) {
        throw std::logic_error("Empty name item");
    } else {
        nameItem_ = name;
    }
}

void Item::setWeight(size_t weight) { weight_ = weight; }

//CartridgeConteiner

CartridgeContainer::CartridgeContainer(std::string const& containerName,  const std::string &cartridgeTypeName,
                                       int maxCartridges, int currentCartridges, int containerWeight):
                                       Item(containerName, containerWeight), type(cartridgeTypeName) {
    if (maxCartridges < currentCartridges) {
        throw std::logic_error("max count less them current");
    }
    maxCartridgeInContainer = maxCartridges;
    currentCartridgeCount = currentCartridges;
}

int CartridgeContainer::getCurrent() noexcept { return currentCartridgeCount; }
int CartridgeContainer::getMaxCartridgeInContainer() noexcept { return maxCartridgeInContainer; }

void CartridgeContainer::giveCartridges(int count) {
    if(count <= currentCartridgeCount) {
        currentCartridgeCount -= count;
    } else {
        throw std::logic_error("count more curren count");
    }
}

cartridgeType CartridgeContainer::getType() noexcept { return type; }

//Weapon

Weapon::Weapon(): Item("", 0) {}

Weapon::Weapon(std::string const& name, std::string const& baseName) : Item(name, 0) {
    auto const& iter = baseWeapon.find(baseName);
    if(iter == baseWeapon.end()) {
        throw std::logic_error("It's not a basic Weapon");
    }
    Weapon const& sec = iter->second;
    weight_ = sec.weight_;
    damage_ = sec.damage_;
    shotTime_ = sec.shotTime_;
    reloadTime_ = sec.reloadTime_;
    cartridgeType_ = sec.cartridgeType_;
    maxCartridgeInWeapon_ = sec.maxCartridgeInWeapon_;
    currentCartridgeWeapon_ = 0;
}

Weapon::Weapon(std::string const& name, size_t weight, int damage, int shotTime, int reloadTime,
               std::string type, int maxCartridgeInWeapon) :
               Item(name, weight), damage_(damage), shotTime_(shotTime), reloadTime_(reloadTime), cartridgeType_(type), maxCartridgeInWeapon_(maxCartridgeInWeapon) {}

void Weapon::addWeapon(const std::string &name, size_t weight, int damage, int shotTime, int reloadTime,
                       std::string type, int maxCartridgeInWeapon) {
    if(!baseWeapon.count(name)) {
        baseWeapon[name] = Weapon(name, weight, damage, shotTime, reloadTime, type, maxCartridgeInWeapon);
    } else {
        throw std::logic_error("Basic weapon " + name + " already exists");
    }
}

int Weapon::shot() {
    if(currentCartridgeWeapon_ == 0) {
        throw std::logic_error("cartridge container in " + nameItem_ + " is empty");
    }
    currentCartridgeWeapon_--;
    return shotTime_;
}

int Weapon::reload(CartridgeContainer &container) {
    if(container.getType() != cartridgeType_) {
        throw std::logic_error("invalid container type");
    }
    int needCartridges;
    if(container.getCurrent() > 0) {
        container.getCurrent() < maxCartridgeInWeapon_ ?
        container.giveCartridges(container.getCurrent()) : container.giveCartridges(maxCartridgeInWeapon_);
    }
    return reloadTime_;
}

int Weapon::getDamage() noexcept { return  damage_; }
int Weapon::getShotTime() noexcept { return  shotTime_; }
int Weapon::getReloadTime() noexcept { return reloadTime_; }
cartridgeType Weapon::getCartridgeType() noexcept { return cartridgeType_; }
int Weapon::getMaxCartridges() noexcept { return maxCartridgeInWeapon_; }
int Weapon::getCurrentCartridges() noexcept { return currentCartridgeWeapon_; }

void Weapon::setCurrentCartridges(int count) {
    if(maxCartridgeInWeapon_ < count) {
        throw std::logic_error("more max count");
    }
    currentCartridgeWeapon_ = count;
}

MedKit::MedKit(std::string const& name, int weight, int healthCount) : Item(name, weight) {
    healthCount >= 0 ? countHealth = healthCount : throw std::logic_error("health count less 0");
}

bool MedKit::used() noexcept {
    if(isUsed) {
        return false;
    } else {
        isUsed = true;
        return true;
    }
}

int MedKit::getUseTime() noexcept { return useTime; }
int MedKit::getCountHealth() noexcept { return countHealth; }

// Inventory

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
// Entity

Entity::Entity(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius) :
name_(name), maxHealth_(maxHealth), maxTime_(maxTime), moveTime_(moveTime), viewingRadius_(viewingRadius) {
    currentHealth_ = maxHealth;
}

bool Entity::setHealth(int count) {
    if(count > maxHealth_) {
        return false;
    }
    currentHealth_ = count;
    return true;
}


std::string const& Entity::getName() noexcept { return name_; }
int Entity::getCurrentHealth() const noexcept { return currentHealth_; }
int Entity::getMaxHealth() const noexcept { return maxHealth_; }
int Entity::getCurrentTime() const noexcept { return currentTime_; }
int Entity::getMaxTime() const noexcept { return  maxTime_; }
int Entity::getMoveTime() const noexcept { return moveTime_; }
int Entity::getViewingRadius() const noexcept { return viewingRadius_; }

// Attacking

Attacking::Attacking(int damage, double accuracy) {
    if(damage >= 0 && accuracy >= 0) {
        this->damage = damage;
        this->accuracy = accuracy;
    } else {
        throw std::logic_error("invalid value");
    }
}

int Attacking::getDamage() { return damage; }
double Attacking::getAccuracy() { return accuracy; }

// wildEntity

int wildEntity::attack() {

}


int wildEntity::move() {
    if(currentTime_ < moveTime_) {
        throw std::logic_error("not enough time points");
    }
    currentTime_ -= moveTime_;
    return moveTime_;
}

// SmartEntity

SmartEntity::SmartEntity() : Entity(), Attacking(0, 0),attackTime(0) {}

SmartEntity::SmartEntity(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius) :
Entity(name, maxHealth, maxTime, moveTime, viewingRadius), Attacking(0, 0) {
    attackTime = 0;
}

int SmartEntity::move() {
    if(currentTime_ < moveTime_) {
        throw std::logic_error("not enough time points");
    }
    currentTime_ -= moveTime_;
    return moveTime_;
}

bool SmartEntity::canKeeping(Item *item) {
    if(dynamic_cast<Weapon*>(item)) {
        return true;
    }
    return false;
}

Item * SmartEntity::throwItem(std::string const& name) {
    Item * res = activeWeapon;
    activeWeapon = nullptr;
    return res;
}

bool SmartEntity::addItem(Item *item) {
    if(activeWeapon || !canKeeping(item)) {
        return false;
    }
    activeWeapon = dynamic_cast<Weapon*>(item);
    return true;
}

int SmartEntity::attack() {
    if(currentTime_ < attackTime) {
        throw std::logic_error("not enough time points");
    }
    currentTime_ -= attackTime;
    return attackTime;
}

Weapon* SmartEntity::getActiveWeapon() {
    return activeWeapon;
}

Furajire::Furajire(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius, size_t x, size_t y) :
Entity(name, maxHealth, maxTime, moveTime, viewingRadius), inventory(x, y)   {}

int Furajire::move() {
    if(currentTime_ < moveTime_) {
        throw std::logic_error("not enough time points");
    }
    currentTime_ -= moveTime_;
    return moveTime_;
}

Item* Furajire::throwItem(std::string const& name) {
    for (auto const i : inventory.data()) {
        for (int j = 0; j < inventory.data().size().second; j++) {
            if(i[j]->getName() == name) {
                Item *res = i[j];
                i[j] = nullptr;
                return res;
            }
        }
    }
    throw std::runtime_error("\"" + name + "\" not found");
}

bool Furajire::addItem(Item *item) {
    if(!inventory.isEmpty()) {
        inventory.addItemToInventory(item);
        return true;
    }
    return false;
}

// Operatives

Operative::Operative() : SmartEntity(), inventory_(0,0) {}

Operative::Operative(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius) :
    inventory_(2, 2), SmartEntity(name, maxHealth, maxTime, moveTime, viewingRadius){

}

int Operative::getWeight() noexcept {
    int res = 0;
    for (auto const i : inventory_.data()) {
        for (int j = 0; j < inventory_.data().size().second; j++) {
            if(i[j] != nullptr) {
                res += i[j]->getWeight();
            }
        }
    }
    return res;
}

int Operative::getTotalWeight() noexcept { return totalWeight; }

std::pair<bool, int> Operative::useMedKit() {
    int needHealthCount = 0;
    MedKit * needMedKit = nullptr;
    int difference = 0;
    size_t x = 0 , y = 0;
    for (int i = 0; i < inventory_.data().size().first; i++) {
        for (int j = 0; j < inventory_.data().size().second; ++j) {
            MedKit * medKit = dynamic_cast<MedKit*>(inventory_.data()[i][j]);
            if(medKit) {
                if(needHealthCount == 0) {
                    needHealthCount = medKit->getCountHealth();
                    needMedKit = medKit;
                    difference = abs(needHealthCount - medKit->getCountHealth());
                    x = i; y = j;
                } else {
                    difference = abs(medKit->getCountHealth() - needHealthCount) < difference ?
                        abs(medKit->getCountHealth() - needHealthCount) : difference;
                    needMedKit = abs(medKit->getCountHealth() - needHealthCount) < difference ? medKit : needMedKit;
                    if(abs(medKit->getCountHealth() - needHealthCount) < difference) {
                        x = i;
                        y = j;
                    }
                }
            }
        }
    }
    int time;
    if(needMedKit) {
        currentHealth_ += needMedKit->getCountHealth() > maxHealth_ - currentHealth_ ?
            maxHealth_ - currentHealth_ : needMedKit->getCountHealth();
        time = needMedKit->getUseTime();
        delete inventory_.data()[x][y];
        inventory_.data()[x][y] = nullptr;
    } else {
        return {false, 0};
    }
    return {true, time};
}

bool Operative::addItem(Item* item) {
    for (int i = 0; i < inventory_.data().size().first; ++i) {
        for (int j = 0; j < inventory_.data().size().second; ++j) {
            if(!inventory_.data()[i][j]) {
                inventory_.data()[i][j] = item;
                return true;
            }
        }
    }
    return false;
}


// Level

std::vector<Item *> & Square::getItems() noexcept {
    return items;
}

SquareType Square::getSquareType() {
    return type;
}

void Square::changeSquareType(SquareType newtype) {
    type = newtype;
}

std::pair<size_t, size_t> Level::size() const noexcept {
    return gameField.size();
}


bool Level::setSize(size_t x, size_t y) {
    if(x < gameField.size().first || y < gameField.size().second) {
        return false;
    }
    try {
        Matrix<Square> copy(x, y);
        for (int i = 0; i < gameField.size().first; ++i) {
            for (int j = 0; j < gameField.size().second; ++j) {
                copy[i][j] = std::move(gameField[i][j]);
            }
        }
        gameField = copy;
    } catch (std::exception const& r){
        //Добавить log игры
        return false;
    }
    return true;
}

bool Square::addEntity(Entity* entity) {
    if(entity_) {
        throw std::logic_error("this square isn't empty");
    }
    entity_ = entity;
    //std::cout << entity_;
    return true;
}

Entity* Square::deleteEntity() {
    Entity * res = entity_;
    entity_ = nullptr;
    return res;
}


Entity* Square::getEntity() {
    return entity_;
}

void Level::setSquareType(SquareType type, size_t x, size_t y) {
    if(x < gameField.size().first && y < gameField.size().second) {
        gameField[x][y].changeSquareType(type);
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}

std::vector<Item*> Level::deleteItemFromSquare(size_t x, size_t y) {
    if(x < gameField.size().first && y < gameField.size().second) {
        return std::move(gameField[x][y].getItems());
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}

void Level::addItemToSquare(Item * item, size_t x, size_t y) {
    if(x < gameField.size().first && y < gameField.size().second) {
        gameField[x][y].getItems().push_back(item);
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}

Matrix<Square> & Level::getGameField(){
    return gameField;
}

/*
bool Level::deleteEntity(std::string name) {
    bool flag = false;
    for (auto & i : operatives) {
        if(i->getName() == name) {
            Entity *del = i;
            delete i; // ???
            operatives.erase(i);
            flag = true;
        }
    }
    if(!flag) {
        for (auto & i : monsters) {
            if(i->getName() == name) {
                Entity *del = i;
                delete i; // ???
                operatives.erase(i);
                flag = true;
            }
        }
    }
    return flag;
}

bool Level::deleteEntity(Entity* entity) {
    if(operatives.count(entity)) {
        operatives.erase(entity);
        return true;
    } else if (monsters.count(entity)) {
        monsters.erase(entity);
        return true;
    }
    return false;
}

*/

GameService::GameService() {
    int x = 10, y = 10;
    level.setSize(x, y);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            level.setSquareType(SquareType::Floor, i, j);
        }
    }
}

std::pair<size_t, size_t> findPos(Entity const* entity, Matrix<Square> & gameField) {
    for (int i = 0; i < gameField.size().first; ++i) {
        for (int j = 0; j < gameField.size().second; ++j) {
            if(gameField[i][j].getEntity() == entity) {
                return {i, j};
            }
        }
    }
    throw std::runtime_error("Entity not found");
}

bool AttackService::attack(Entity* entity, Directions const direction) {
    if(dynamic_cast<wildEntity*>(entity)) {
        std::pair<size_t, size_t> entityPos;
        try {
            entityPos = findPos(entity, gameService->getLevel().getGameField());
        } catch (std::exception &) {
            return false;
        }
        Entity* victim = nullptr;
        switch (direction) {
            case Directions::north : {
                if(entityPos.first == 0) { throw std::logic_error("first row and north"); }
                victim = gameService->getLevel().getGameField()[entityPos.first - 1][entityPos.second].getEntity();
                break;
            }
            case Directions::east : {
                if(entityPos.second == gameService->getLevel().getGameField().size().second - 1) {throw std::logic_error("end column and east");}
                victim = gameService->getLevel().getGameField()[entityPos.first][entityPos.second + 1].getEntity();
                break;
            }
            case Directions::south : {
                if(entityPos.first == gameService->getLevel().getGameField().size().first - 1) {throw std::logic_error("end row and south"); }
                victim = gameService->getLevel().getGameField()[entityPos.first + 1][entityPos.second].getEntity();
                break;
            }
            case Directions::west : {
                if(entityPos.second == 0) {throw std::logic_error("first column and west"); }
                victim = gameService->getLevel().getGameField()[entityPos.first - 1][entityPos.second].getEntity();
                break;
            }
        }

        if(dynamic_cast<Operative*>(victim)) {
            //Почему не работает??? (обман, работает)
        }

        auto * wild = dynamic_cast<wildEntity*>(entity);
        victim->setHealth(victim->getCurrentHealth() - wild->getDamage());
        if(victim->getCurrentHealth() <= 0) {
            // death
        }
    }
    return true;
}

/*
    A* algorithm
 */

bool checkSquare(size_t x, size_t y, Level & level, std::initializer_list<SquareType> const acceptTypes) {
    if(x >= level.size().first || y >= level.size().second) return false;
    for (auto const& i : acceptTypes) {
        if(level.getGameField()[x][y].getSquareType() == i) {
            return true;
        }
    }
    return false;
}

struct elementQueue{
    long grade = 0;
    long x = 0;
    long y = 0;

    bool operator==(const elementQueue& elem) const {
        return elem.x == x && elem.y == y;
    }
};

long heuristic(long x, long y, elementQueue const& finish) {
    return abs(x - finish.x) + abs(y - finish.y);
}

std::vector<Square *> MoveAIService::findMinWay(size_t x1, size_t y1, size_t x2, size_t y2) {
    if(!checkSquare(x1, y1, gameService->getLevel(), {SquareType::Storage, SquareType::Floor}) ||
        !checkSquare(x2, y2, gameService->getLevel(), {SquareType::Storage, SquareType::Floor})) {
        throw std::logic_error("invalid coordinates");
        }

    auto cmp = [](elementQueue & elem1, elementQueue & elem2) {return elem1.grade > elem2.grade;};

    std::priority_queue<elementQueue, std::vector<elementQueue>, decltype(cmp)> queue(cmp);

    elementQueue start{0, long(x1), long(y1)};
    elementQueue finish{0, long(x2), long(y2)};
    Matrix<std::pair<size_t, size_t>> prev(gameService->getLevel().size().first, gameService->getLevel().size().second);
    Matrix<long> weights(gameService->getLevel().size().first, gameService->getLevel().size().second, LONG_MAX);
    std::unordered_set<Square*> visited;

    queue.emplace(start);
    weights[start.x][start.y] = 0;

    while (!queue.empty()) {
        elementQueue work = queue.top();

        if(work == finish) {
            break;
        }
        if(checkSquare(work.x, work.y - 1, gameService->getLevel(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x][work.y - 1] > work.grade + heuristic(work.x, work.y - 1, finish) + 1) {
                weights[work.x][work.y - 1] = work.grade + heuristic(work.x, work.y - 1, finish) + 1;
                prev[work.x][work.y - 1] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x, work.y - 1, finish) + 1, work.x, work.y - 1});
            }
        }

        if(checkSquare(work.x, work.y + 1, gameService->getLevel(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x][work.y + 1] > work.grade + heuristic(work.x, work.y + 1, finish) + 1) {
                weights[work.x][work.y + 1] = work.grade + heuristic(work.x, work.y + 1, finish) + 1;
                prev[work.x][work.y + 1] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x, work.y + 1, finish) + 1, work.x, work.y + 1});
            }
        }

        if(checkSquare(work.x - 1, work.y, gameService->getLevel(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x - 1][work.y] > work.grade + heuristic(work.x - 1, work.y, finish) + 1) {
                weights[work.x - 1][work.y] = work.grade + heuristic(work.x - 1, work.y, finish) + 1;
                prev[work.x - 1][work.y] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x - 1, work.y, finish) + 1, work.x - 1, work.y});
            }
        }

        if(checkSquare(work.x + 1, work.y, gameService->getLevel(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x + 1][work.y] > work.grade + heuristic(work.x + 1, work.y, finish) + 1) {
                weights[work.x + 1][work.y] = work.grade + heuristic(work.x + 1, work.y, finish) + 1;
                prev[work.x + 1][work.y] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x + 1, work.y, finish) + 1, work.x + 1, work.y});
            }
        }
        queue.pop();
    }


    if(weights[finish.x][finish.y] == LONG_MAX) {
        return {};
    }

    elementQueue i = {0, finish.x, finish.y};
    elementQueue swap;
    std::vector<Square*> result;
    while (i != start) {
        result.push_back(&gameService->getLevel().getGameField()[i.x][i.y]);
        swap = i;
        i.x = prev[swap.x][swap.y].first;
        i.y = prev[swap.x][swap.y].second;
    }
    result.push_back(&gameService->getLevel().getGameField()[start.x][start.y]);

    return result;
}