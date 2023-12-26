#ifndef GAMESERVICES_H#define GAMESERVICES_H#include "enemy.h"#include "matrix_lib.h"#include "items.h"#include <set>#include <queue>#include <memory>enum class SquareType {    Floor,    Wall,    Window,    Barrier,    Storage,    NoType};class Square{public:    Square() = default;    [[nodiscard]] Entity* getEntity() const;    [[nodiscard]] std::vector<Item *> & getItems() noexcept;    bool addEntity(Entity* entity);    Entity* deleteEntity();    [[nodiscard]] SquareType getSquareType() const;    std::vector<Item*> deleteAllItems() noexcept;    Item* deleteItem(std::string const& name) noexcept;    void changeSquareType(SquareType newtype);private:    std::vector<Item *> items = std::vector<Item *>();    SquareType type = SquareType::Floor;    Entity *entity_ = nullptr;};class Level {public:    Level(size_t x, size_t y);    Matrix<Square*> & getGameField();    [[nodiscard]] std::pair<size_t, size_t> size() const noexcept;    [[nodiscard]] std::pair<size_t, size_t> findSquare(Square * square);    bool setSize(size_t x, size_t y);    void setSquareType(SquareType type, size_t x, size_t y);    std::vector<Item*> deleteItemsFromSquare(size_t x, size_t y);    Item* deleteItemFromSquare(size_t x, size_t y, std::string const& name);    void addItemToSquare(Item * item, size_t x, size_t y);    void addItemToSquare(std::vector<Item*> const& items, size_t x, size_t y);    bool addEntity(Entity * entity, size_t x, size_t y);    bool deleteEntity(std::string const& name);    bool deleteEntity(Entity * entity);    [[nodiscard]] std::set<Entity*> const& getMonsters() const;    [[nodiscard]] std::set<Entity*> const& getOperatives() const;private:    std::set<Entity*> operatives = std::set<Entity*>();    std::set<Entity*> monsters = std::set<Entity*>();    Matrix<Square*> gameField;};class GameService {public:    GameService();    bool upload();    bool load();    Level & getLevel() {return level;}protected:    Level level;};[[nodiscard]] std::vector<Square*> findMinWay(size_t x1, size_t y1, size_t x2, size_t y2, Level & level);class AttackService {public:    explicit AttackService(std::shared_ptr<GameService> const& game);    bool attack(Entity * entity, Directions direction);protected:    std::shared_ptr<GameService> gameService_;};class MoveService {public:    explicit MoveService(std::shared_ptr<GameService> const& game);    bool move(Entity * entity, Directions direction);protected:    std::shared_ptr<GameService> gameService_;};struct FirePlace {    size_t x_;    size_t y_;    Directions directions_;    int lenght;    bool result;};class EntityAI : public MoveService, public AttackService {public:    explicit EntityAI(std::shared_ptr<GameService> const& game);    void AITick();    FirePlace findFirePlace(Entity * entity, Entity * victim);    void walkingAttackingEntities();    void walkingNearestAttackingEntities();    bool findAttackingNoSmart(Entity * entity);    Weapon * findWeaponInStorages(Entity * entity);private:    std::shared_ptr<GameService> gameService_;};std::vector<Entity*> entityScanerRadius(Entity * entity, int x, int y, Matrix<Square*> & field);bool viewingObjectArea(int start_x, int start_y, int target_x, int target_y, Matrix<Square> & field);#endif //GAMESERVICES_H