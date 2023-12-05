#include "enemy.h"

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
    if(inventory_.addItemToInventory(item)) {
        totalWeight += item->getWeight();
        return true;
    }
    return false;
}