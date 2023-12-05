#ifndef GAMESERVICES_H
#define GAMESERVICES_H

#include "enemy.h"
#include "matrix_lib.h"
#include "items.h"
#include "Inventory.h"
#include <set>
#include <queue>

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

class MoveService {
public:
    MoveService() = default;
    std::vector<Square*> findMinWay(size_t x1, size_t y1, size_t x2, size_t y2);
    bool move(Entity *, Directions direction);

    GameService *gameService  = new GameService();
};

#endif //GAMESERVICES_H
