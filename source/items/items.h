#ifndef ITEMS_H
#define ITEMS_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum class Directions {
    north, east, south, west
};

class AmmoTypeRepo {
public:
    AmmoTypeRepo();
    static AmmoTypeRepo& instance();
    bool find(std::string const& name);
    bool insert(std::string const& name);

private:
    static std::unordered_set<std::string> names_;
};

class cartridgeType {
public:
    cartridgeType();
    explicit cartridgeType(std::string const& name);
    explicit cartridgeType(std::vector<std::string> const& names); // Для инициализации названий существующих патронов

    void addCartridgeType(std::string const& name);
    bool operator==(cartridgeType const& type);
    bool operator!=(cartridgeType const& type);

    [[nodiscard]] std::string const& getName() const noexcept;

private:
    AmmoTypeRepo typesRepo = AmmoTypeRepo();
    std::string name_;
};

class Item {
public:
    Item() noexcept;
    Item(std::string const& nameItem, size_t weight);

    std::string const& getName();
    size_t getWeight();

    void setName(std::string const& name);
    void setWeight(size_t weight);

    virtual ~Item() = default;
protected:
    std::string nameItem_;
    size_t weight_ = 0;
};

class CartridgeContainer : public Item {
public:
    CartridgeContainer(std::string const& containerName,  const std::string &cartridgeTypeName, int maxCartridges, int currentCartridges, int containerWeight);

    int getCurrent() noexcept;
    int getMaxCartridgeInContainer() noexcept;

    cartridgeType getType() noexcept;

    void giveCartridges(int count);

private:
    cartridgeType type;
    int maxCartridgeInContainer;
    int currentCartridgeCount;
};

class Weapon : public Item {
public:
    Weapon();
    Weapon(std::string const& name, std::string const& baseName);

    void addWeapon(std::string const& name, size_t weight, int damage, int shotTime, int reloadTime, std::string type,
                   int maxCartridgeInWeapon);

    int shot(); // возвращает время затрачиваемое на выстрел
    int reload(CartridgeContainer & container); // возвращает время затрачиваемое на перезарядку

    int getDamage() noexcept;
    int getShotTime() noexcept;
    int getReloadTime() noexcept;
    cartridgeType getCartridgeType() noexcept;
    int getMaxCartridges() noexcept;
    int getCurrentCartridges() noexcept;
    void setCurrentCartridges(int count);

private:
    Weapon(std::string const& name, size_t weight, int damage, int shotTime, int reloadTime, std::string type,
           int maxCartridgeInWeapon);
    int damage_ = 0;
    int shotTime_ = 0;
    int reloadTime_ = 0;
    cartridgeType cartridgeType_ = cartridgeType("");
    int maxCartridgeInWeapon_ = 0;
    int currentCartridgeWeapon_ = 0;
    static std::unordered_map<std::string, Weapon> baseWeapon;
};

class MedKit : public Item {
public:
    MedKit();
    MedKit(std::string const& name, int weight, int healthCount);

    bool used() noexcept; // true - была не использована, использована в итоге false - использовалась

    int getUseTime() noexcept;
    int getCountHealth() noexcept;

    ~MedKit() override = default;
private:
    int useTime = 0;
    int countHealth = 0;
    bool isUsed = false;
};

#endif //ITEMS_H
