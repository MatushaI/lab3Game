#ifndef GAMESERVICES_H
#define GAMESERVICES_H

#include "enemy.h"
#include "matrix_lib.h"
#include "items.h"
#include <set>
#include <queue>

enum class SquareType {
    Floor,
    Wall,
    Window,
    Barrier,
    Storage,
    NoType
};

class Square{
public:
    Square() = default;

    [[nodiscard]] Entity* getEntity() const;
    [[nodiscard]] std::vector<Item *> & getItems() noexcept;
    bool addEntity(Entity* entity);
    Entity* deleteEntity();
    [[nodiscard]] SquareType getSquareType() const;
    void changeSquareType(SquareType newtype);

private:
    std::vector<Item *> items = std::vector<Item *>();
    SquareType type = SquareType::Floor;
    Entity *entity_ = nullptr;
};

class Level {
public:
    Level(size_t x, size_t y);
    Matrix<Square*> & getGameField();
    [[nodiscard]] std::pair<size_t, size_t> size() const noexcept;
    [[nodiscard]] std::pair<size_t, size_t> findSquare(Square * square);
    bool setSize(size_t x, size_t y);
    void setSquareType(SquareType type, size_t x, size_t y);
    std::vector<Item*> deleteItemFromSquare(size_t x, size_t y);
    void addItemToSquare(Item * item, size_t x, size_t y);
    void addItemToSquare(std::vector<Item*> const& items, size_t x, size_t y);
    bool addEntity(Entity * entity, size_t x, size_t y);
    bool deleteEntity(std::string const& name);
    bool deleteEntity(Entity * entity);
    [[nodiscard]] std::set<Entity*> const& getMonsters() const;
    [[nodiscard]] std::set<Entity*> const& getOperatives() const;
private:
    std::set<Entity*> operatives = std::set<Entity*>();
    std::set<Entity*> monsters = std::set<Entity*>();
    Matrix<Square*> gameField;
};

class GameService {
public:
    GameService();
    bool upload();
    bool load();
    Level & getLevel() {return level;} // для отладки
protected:
    Level level;
};

class AttackService {
public:
    explicit AttackService(GameService * game);
    bool attack(Entity * entity, Directions direction);
    GameService * gameService = nullptr; // убрать
};



class MoveService {
public:
    explicit MoveService(GameService * game);
    MoveService() = default;
    [[nodiscard]] std::vector<Square*> findMinWay(size_t x1, size_t y1, size_t x2, size_t y2) const;
    bool move(Entity * entity, Directions direction);

    GameService *gameService = nullptr;
};

class EntityAI {
public:
    explicit EntityAI(GameService * game, MoveService * moveS);
    void AITick();
private:
    GameService * game = nullptr;
    MoveService * moveS = nullptr;
};

std::vector<Entity*> entityScanerRadius(Entity * entity, int x, int y, Matrix<Square*> & field);
bool viewingObjectArea(int start_x, int start_y, int target_x, int target_y, Matrix<Square> & field);

#endif //GAMESERVICES_H
