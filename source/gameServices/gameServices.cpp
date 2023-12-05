#include "gameServices.h"

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

std::vector<Square *> MoveService::findMinWay(size_t x1, size_t y1, size_t x2, size_t y2) {
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