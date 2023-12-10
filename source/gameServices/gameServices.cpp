#include "gameServices.h"

#include <__ranges/drop_view.h>

std::vector<Item *> & Square::getItems() noexcept {
    return items;
}

SquareType Square::getSquareType() const {
    return type;
}

void Square::changeSquareType(SquareType const newtype) {
    type = newtype;
}

std::pair<size_t, size_t> Level::size() const noexcept {
    return gameField.size();
}

bool Level::setSize(size_t const x, size_t const y) {
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
    } catch (std::exception const&){
        return false;
    }
    return true;
}

bool Square::addEntity(Entity* entity) {
    if(entity_) {
        throw std::logic_error("this square isn't empty");
    }
    entity_ = entity;
    return true;
}

Entity* Square::deleteEntity() {
    Entity * res = entity_;
    entity_ = nullptr;
    return res;
}

Entity* Square::getEntity() const{
    return entity_;
}

void Level::setSquareType(SquareType const type, size_t const x, size_t const y) {
    if(x < gameField.size().first && y < gameField.size().second) {
        gameField[x][y].changeSquareType(type);
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}

std::vector<Item*> Level::deleteItemFromSquare(size_t const x, size_t const y) {
    if(x < gameField.size().first && y < gameField.size().second) {
        return std::move(gameField[x][y].getItems());
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}

void Level::addItemToSquare(Item * item, size_t const x, size_t const y) {
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

bool deleteEntityFromField (Entity * entity, Matrix<Square> & field) {
    for (int i = 0; i < field.size().first; i++) {
        for (int j = 0; j < field.size().second; j++) {
            if(field[i][j].getEntity() == entity) {

                field[i][j].deleteEntity();
                return true;
            }
        }
    }
    return false;
}

bool Level::addEntity(Entity* entity, size_t x, size_t y) {
    try {
        gameField[x][y].addEntity(entity);
        if(dynamic_cast<Operative*>(entity)) {
            operatives.insert(entity);
        } else {
            monsters.insert(entity);
        }
    } catch (std::logic_error const&) {
        return false;
    }
    return true;
}


bool Level::deleteEntity(std::string const& name) {
    auto it =  std::find_if(operatives.begin(), operatives.end(), [name](Entity * entity){return entity->getName() == name;});
    if(it != operatives.end()) {
        deleteEntityFromField(*it, gameField);
        delete *it;
        operatives.erase(it);
        return true;
    }
    it =  std::find_if(monsters.begin(), monsters.end(), [name](Entity * entity){return entity->getName() == name;});
    if(it != monsters.end()) {
        deleteEntityFromField(*it, gameField);
        delete *it;
        monsters.erase(it);
        return true;
    }
    return false;
}

bool Level::deleteEntity(Entity* entity) {
    if(operatives.count(entity)) {
        deleteEntityFromField(entity, gameField);
        operatives.erase(entity);
        delete entity;
        return true;
    }

    if(monsters.count(entity)) {
        deleteEntityFromField(entity, gameField);
        monsters.erase(entity);
        delete entity;
        return true;
    }
    return false;
}


bool AttackService::attack(Entity* entity, Directions const direction){

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

    if((dynamic_cast<Operative*>(victim) && dynamic_cast<Operative*>(entity)) ||
        (!dynamic_cast<Operative*>(victim) && !dynamic_cast<Operative*>(entity)) ||
        victim == nullptr) {
        return false;
    }

    auto * attackingEntity = dynamic_cast<Attacking*>(entity);

    int damage = attackingEntity->attack();

    if(victim->getCurrentHealth() <= damage) {
        gameService->getLevel().deleteEntity(victim);
    } else {
        victim->setHealth(victim->getCurrentHealth() - damage);
    }
    return true;
}

/*
    A* algorithm
 */

bool checkSquare(size_t x, size_t y, Matrix<Square> & field, std::initializer_list<SquareType> const acceptTypes) {
    if(x >= field.size().first || y >= field.size().second) return false;
    for (auto const& i : acceptTypes) {
        if(field[x][y].getSquareType() == i) {
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

AttackService::AttackService(GameService* game) {
    gameService = game;
}

MoveService::MoveService(GameService* game) {
    gameService = game;
}

std::vector<Square *> MoveService::findMinWay(size_t x1, size_t y1, size_t x2, size_t y2) const {
    if(!checkSquare(x1, y1, gameService->getLevel().getGameField(), {SquareType::Storage, SquareType::Floor}) ||
        !checkSquare(x2, y2, gameService->getLevel().getGameField(), {SquareType::Storage, SquareType::Floor})) {
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
        if(checkSquare(work.x, work.y - 1, gameService->getLevel().getGameField(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x][work.y - 1] > work.grade + heuristic(work.x, work.y - 1, finish) + 1) {
                weights[work.x][work.y - 1] = work.grade + heuristic(work.x, work.y - 1, finish) + 1;
                prev[work.x][work.y - 1] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x, work.y - 1, finish) + 1, work.x, work.y - 1});
            }
        }

        if(checkSquare(work.x, work.y + 1, gameService->getLevel().getGameField(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x][work.y + 1] > work.grade + heuristic(work.x, work.y + 1, finish) + 1) {
                weights[work.x][work.y + 1] = work.grade + heuristic(work.x, work.y + 1, finish) + 1;
                prev[work.x][work.y + 1] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x, work.y + 1, finish) + 1, work.x, work.y + 1});
            }
        }

        if(checkSquare(work.x - 1, work.y, gameService->getLevel().getGameField(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x - 1][work.y] > work.grade + heuristic(work.x - 1, work.y, finish) + 1) {
                weights[work.x - 1][work.y] = work.grade + heuristic(work.x - 1, work.y, finish) + 1;
                prev[work.x - 1][work.y] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x - 1, work.y, finish) + 1, work.x - 1, work.y});
            }
        }

        if(checkSquare(work.x + 1, work.y, gameService->getLevel().getGameField(), {SquareType::Storage, SquareType::Floor})) {
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

bool MoveService::move(Entity* entity, Directions direction) {

    std::pair<size_t, size_t> entityPos;
    try {
        entityPos = findPos(entity, gameService->getLevel().getGameField());
    } catch (std::exception &) {
        return false;
    }

    switch (direction) {
        case Directions::north : {
            if(entityPos.first == 0) { throw std::logic_error("first row and north"); }
            if(gameService->getLevel().getGameField()[entityPos.first - 1][entityPos.second].getEntity() == nullptr) {
                gameService->getLevel().getGameField()[entityPos.first][entityPos.second].deleteEntity();
                gameService->getLevel().getGameField()[entityPos.first - 1][entityPos.second].addEntity(entity);
            }
            break;
        }
        case Directions::east : {
            if(entityPos.second == gameService->getLevel().getGameField().size().second - 1) {throw std::logic_error("end column and east");}
            if(gameService->getLevel().getGameField()[entityPos.first][entityPos.second + 1].getEntity() == nullptr) {
                gameService->getLevel().getGameField()[entityPos.first][entityPos.second].deleteEntity();
                gameService->getLevel().getGameField()[entityPos.first][entityPos.second + 1].addEntity(entity);
            }
            break;
        }
        case Directions::south : {
            if(entityPos.first == gameService->getLevel().getGameField().size().first - 1) {throw std::logic_error("end row and south"); }
            if(gameService->getLevel().getGameField()[entityPos.first + 1][entityPos.second].getEntity() == nullptr) {
                gameService->getLevel().getGameField()[entityPos.first][entityPos.second].deleteEntity();
                gameService->getLevel().getGameField()[entityPos.first + 1][entityPos.second].addEntity(entity);
            }
            break;
        }
        case Directions::west : {
            if(entityPos.second == 0) {throw std::logic_error("first column and west"); }
            if(gameService->getLevel().getGameField()[entityPos.first][entityPos.second - 1].getEntity() == nullptr) {
                gameService->getLevel().getGameField()[entityPos.first][entityPos.second].deleteEntity();
                gameService->getLevel().getGameField()[entityPos.first][entityPos.second - 1].addEntity(entity);
            }
            break;
        }
    }
}

EntityAI::EntityAI(GameService* game) : game(game) {}

std::vector<std::pair<size_t, size_t>> findStorages(Matrix<Square> & field) {
    std::vector<std::pair<size_t, size_t>> result;
    for (int i = 0; i < field.size().first; i++) {
        for (int j = 0; j < field.size().second; j++) {
            if(field[i][j].getSquareType() == SquareType::Storage) {
                result.push_back({i, j});
            }
        }
    }
    return result;
}

// Функция для проверки того, видно ли объект (последняя точка не учитывается)

bool viewingObjectArea(int start_x, int start_y, int target_x, int target_y, Matrix<Square> & field) {
    int x = start_x, y = start_y;
    double tan45 = atan2(1, 1);
    if(start_x >= target_x) {
        if(start_y <= target_y) {
            double downTan = atan2(start_x - target_x - 0.5, target_y - start_y + 0.5);
            double upTan = atan2(start_x - target_x + 0.5, target_y - start_y - 0.5);

            do {
                atan2(start_x - x + 0.5, y - start_y + 0.5) > downTan ? y++ : x--;

                if(!checkSquare(x, y, field, {SquareType::Floor, SquareType::Storage, SquareType::Window})) {
                    downTan = atan2( start_x - x + 0.5, y - start_y - 0.5);
                }
                if(downTan > upTan) {
                    return false;
                }
            } while (!(x == target_x && y == target_y));

            return true;
        } else {
            double downTan = atan2(start_x - target_x - 0.5, target_y - start_y - 0.5);
            double upTan = atan2(start_x - target_x + 0.5, target_y - start_y + 0.5);

            do {
                atan2(start_x - x + 0.5, y - start_y - 0.5) < downTan ? y-- : x--;

                if(!checkSquare(x, y, field, {SquareType::Floor, SquareType::Storage, SquareType::Window})) {
                    downTan = atan2( start_x - x + 0.5, y - start_y - 0.5);
                }
                if(downTan < upTan) {
                    return false;
                }
            } while (!(x == target_x && y == target_y));

            return true;
        }
    } else {
        if(start_y <= target_y) {
            double downTan = atan2(start_x - target_x - 0.5, target_y - start_y - 0.5);
            double upTan = atan2(start_x - target_x + 0.5, target_y - start_y + 0.5);

            do {
                atan2(start_x - x - 0.5, y - start_y + 0.5) > downTan ? x++ : y++;

                if(!checkSquare(x, y, field, {SquareType::Floor, SquareType::Storage, SquareType::Window})) {
                    downTan = atan2( start_x - x - 0.5, y - start_y + 0.5);
                }
                if(downTan > upTan) {
                    return false;
                }
            } while (!(x == target_x && y == target_y));

            return true;
        } else {
            double downTan = atan2(start_x - target_x - 0.5, target_y - start_y + 0.5);
            double upTan = atan2(start_x - target_x + 0.5, target_y - start_y - 0.5);

            do {
                atan2(start_x - x - 0.5, y - start_y + 0.5) < downTan ? x++ : y--;

                if(!checkSquare(x, y, field, {SquareType::Floor, SquareType::Storage, SquareType::Window})) {
                    downTan = atan2( start_x - x - 0.5, y - start_y + 0.5);
                }
                if(downTan < upTan) {
                    return false;
                }
            } while (!(x == target_x && y == target_y));

            return true;
        }
    }
}

std::vector<Entity*> entityScanerRadius(Entity * entity, int x, int y, Matrix<Square> & field) {
    std::vector<Entity*> result;
    int viewingRadius = entity->getViewingRadius();
    if(x < 0 || y < 0 || x >= field.getColumns() || y >= field.getRows()) {
        return result;
    }

    int start_x = x - viewingRadius < 0 ? 0 : x - viewingRadius;
    int finish_x = x + viewingRadius >= field.getColumns() ? static_cast<int>(field.getColumns()) - 1 : x + viewingRadius;

    int start_y = y - viewingRadius < 0 ? 0 : y - viewingRadius;
    int finish_y = y + viewingRadius >= field.getRows() ? static_cast<int>(field.getRows()) - 1 : y + viewingRadius;

    for (int i = start_x; i <= finish_x; i++) {
        for (int j = start_y; j <= finish_y; j++) {
            if(sqrt(pow(x - i, 2) + pow(y - j, 2)) <= viewingRadius) {
                if(field[i][j].getEntity() && !(i == x && j == y)) {
                    if(viewingObjectArea(x, y, i, j, field)) {
                        result.push_back(field[i][j].getEntity());
                    }
                }
            }
        }
    }

    return result;
}

void EntityAI::AITick() {
    std::vector<std::pair<size_t, size_t>> storagePoints = findStorages(game->getLevel().getGameField());

}


