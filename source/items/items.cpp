#include "items.h"

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

