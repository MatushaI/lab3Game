#ifndef ENEMY_H
#define ENEMY_H

#include "items.h"
#include "Inventory.h"

class ActionItem {
public:
    virtual Item* throwItem(std::string const& name) = 0;
    virtual bool addItem(Item * item) = 0;
    virtual bool canKeeping(Item * item) = 0;
    virtual ~ActionItem() = default;
};

class Attacking {
public:
    Attacking(int damage, double accuracy);

    virtual int attack() = 0;
    int getDamage();
    double getAccuracy();
    virtual ~Attacking() = default;
protected:
    int damage;
    double accuracy;
};

class MedKitUsed {
public:
    virtual std::pair<bool, int> useMedKit() = 0;
    virtual ~MedKitUsed() = default;
};

// Entity

class Entity {
public:

    Entity() = default;
    Entity(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius);
    std::string const& getName() noexcept;
    [[nodiscard]] int getCurrentHealth() const noexcept;
    [[nodiscard]] int getMaxHealth() const noexcept;
    [[nodiscard]] int getCurrentTime() const noexcept;
    [[nodiscard]] int getMaxTime() const noexcept;
    [[nodiscard]] int getMoveTime() const noexcept;
    [[nodiscard]] int getViewingRadius() const noexcept;
    bool setHealth(int count);
    void updateCurrentTime();

    virtual int move() = 0;
    virtual ~Entity() = default;
protected:
    std::string name_;
    int currentHealth_ = 0;
    int maxHealth_ = 0;
    int currentTime_ = 0;
    int maxTime_ = 0;
    int moveTime_ = 0;
    int viewingRadius_ = 0;
};

class wildEntity : public Entity, public Attacking {
public:
    wildEntity();
    wildEntity(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius, int damage, double accuracy, int attackTime);
    int attack() override;
    int move() override;
    ~wildEntity() override = default;
private:
    int attackTime;
};

class SmartEntity : public Entity, public ActionItem, public Attacking { // Добавить новый
public:
    SmartEntity();
    SmartEntity(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius, int damage, double accuracy);

    int move() override;
    Item * throwItem(std::string const& name) override;
    bool addItem(Item * item) override;
    int attack() override;
    Weapon * getActiveWeapon();
    bool canKeeping(Item * item) override;
    ~SmartEntity() override = default;
protected:
    Weapon * activeWeapon = nullptr;
};

class Furajire : public Entity, ActionItem {
public:
    bool canKeeping(Item* item) override;
    Furajire(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius, size_t x, size_t y);
    int move() override;
    Item * throwItem(std::string const& name) override;
    bool addItem(Item * item) override;

private:
    Inventory inventory;
};

class Operative : public SmartEntity, MedKitUsed {
public:
    Operative();
    Operative(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius, int damage, double accuracy);
    int getTotalWeight() noexcept;
    bool addActiveItem(Item * item);
    Item * throwItem(std::string const& name) override;
    bool addItem(Item* item) override;
    Weapon * changeActiveWeapon(Item * item);
    std::pair<bool, int> useMedKit() override;
    ~Operative() override = default;
private:
    Inventory inventory_;
    int totalWeight = 0;
};

#endif //ENEMY_H
