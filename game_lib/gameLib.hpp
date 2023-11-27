#ifndef gameLib_hpp
#define gameLib_hpp

#include <iostream>
#include <string>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../lib/matrix_lib.h"

// Items

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

// Inventory

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

// Entities interfaces

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
private:
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
    int attack() override;
    int move() override;
    ~wildEntity() override = default;
private:
    int attackTime;
    Inventory inventory;
};

class SmartEntity : public Entity, ActionItem, Attacking { // Добавить новый
public:
    SmartEntity();
    SmartEntity(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius);

    int move() override;
    Item * throwItem(std::string const& name) override;
    bool addItem(Item * item) override;
    int attack() override;
    Weapon * getActiveWeapon();
    bool canKeeping(Item * item) override;
    ~SmartEntity() override = default;
protected:
    int attackTime;
    Weapon * activeWeapon = nullptr;
};

class Furajire : public Entity, ActionItem {
public:
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
    Operative(std::string const& name, int maxHealth, int maxTime, int moveTime, int viewingRadius);
    int getWeight() noexcept;
    int getTotalWeight() noexcept;
    //void changeActiveItem();
    std::pair<bool, int> useMedKit() override;
    bool addItem(Item* item) override;
    ~Operative() override = default;
private:
    Inventory inventory_;
    int totalWeight = 0;
};

enum class SquareType {
    Floor,
    Wall,
    Window,
    Barrier,
    Storage
};

class Square{
public:
    Square() = default;
    Square & operator=(Square const&) = default;
    Square & operator=(Square &&) = default;
    Square(Square const&) = default;
    Square(Square &&) = default;
    ~Square() = default;

    Entity* getEntity();
    std::vector<Item *> & getItems() noexcept;
    bool addEntity(Entity* entity);
    Entity* deleteEntity();
    SquareType getSquareType();
    void changeSquareType(SquareType newtype);

private:
    std::vector<Item *> items;
    SquareType type;
    Entity *entity_ = nullptr;
};

class Level {
public:
    Level() = default;
    Matrix<Square> & getGameField();
    [[nodiscard]] std::pair<size_t, size_t> size() const noexcept;
    bool setSize(size_t x, size_t y);
    void setSquareType(SquareType type, size_t x, size_t y);
    std::vector<Item*> deleteItemFromSquare(size_t x, size_t y);
    void addItemToSquare(Item * item, size_t x, size_t y);
    bool addEntity(Entity * entity, size_t x, size_t y) {return gameField[x][y].addEntity(entity);} //Переделать не тольео для монстров
    Entity* deleteEntity(std::string name);
    Entity* deleteEntity(Entity * entity);
private:
    std::set<Entity*> operatives = std::set<Entity*>();
    std::set<Entity*> monsters = std::set<Entity*>();
    Matrix<Square> gameField = Matrix<Square>();
};

class GameService {
public:
    GameService();
    bool upload();
    bool load();
    Level & getLevel() {return level;} // для отладки
protected:
    Level level = Level();
};

class AttackService {
public:
    bool attack(Entity * entity, Directions direction);
    GameService * gameService = new GameService(); // убрать
};

class MoveAIService {
public:
    MoveAIService() = default;
    std::vector<Square*> findMinWay(size_t x1, size_t y1, size_t x2, size_t y2);
    bool move(Entity *, Directions direction);

    GameService *gameService  = new GameService();
};



#endif
