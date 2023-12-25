#include "gameServices.h"
#include <algorithm>

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

Level::Level(size_t x, size_t y) {
    gameField = Matrix<Square*>(x, y, nullptr);
    for (int i = 0; i < gameField.getRows(); i++) {
        for (int j = 0; j < gameField.getColumns(); j++) {
            gameField[i][j] = new Square;
        }
    }
}

bool Level::setSize(size_t const x, size_t const y) {
    if(x < gameField.size().first || y < gameField.size().second) {
        return false;
    }
    try {
        Matrix<Square*> copy(x, y, nullptr);
        for (int i = 0; i < copy.getRows(); i++) {
            for (int j = 0; j < copy.getColumns(); j++) {
                gameField[i][j] = new Square;
            }
        }
        for (int i = 0; i < gameField.size().first; ++i) {
            for (int j = 0; j < gameField.size().second; ++j) {
                delete copy[i][j];
                copy[i][j] = gameField[i][j];
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
        gameField[x][y]->changeSquareType(type);
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}

std::vector<Item*> Level::deleteItemFromSquare(size_t const x, size_t const y) {
    if(x < gameField.size().first && y < gameField.size().second) {
        return std::move(gameField[x][y]->getItems());
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}

void Level::addItemToSquare(Item * item, size_t const x, size_t const y) {
    if(x < gameField.size().first && y < gameField.size().second) {
        gameField[x][y]->getItems().push_back(item);
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}

void Level::addItemToSquare(std::vector<Item *> const&items, size_t x, size_t y) {
    if(x < gameField.size().first && y < gameField.size().second) {
        for (auto i: items) {
            gameField[x][y]->getItems().push_back(i);
        }
    } else {
        throw std::logic_error("more i or j in gameField");
    }
}


Matrix<Square*> & Level::getGameField(){
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

GameService::GameService() : level(15, 15) {
    for (int i = 0; i < level.getGameField().getRows(); i++) {
        for (int j = 0; j < level.getGameField().getColumns(); j++) {
            level.setSquareType(SquareType::Floor, i, j);
        }
    }
}

std::pair<size_t, size_t> findPos(Entity const* entity, Matrix<Square*> & gameField) {
    for (size_t i = 0; i < gameField.size().first; i++) {
        for (size_t j = 0; j < gameField.size().second; j++) {
            if(gameField[i][j]->getEntity() == entity) {
                return {i, j};
            }
        }
    }
    throw std::runtime_error("Entity not found");
}

bool deleteEntityFromField (Entity * entity, Matrix<Square*> & field) {
    for (int i = 0; i < field.size().first; i++) {
        for (int j = 0; j < field.size().second; j++) {
            if(field[i][j]->getEntity() == entity) {
                field[i][j]->deleteEntity();
                return true;
            }
        }
    }
    return false;
}

bool Level::addEntity(Entity* entity, size_t x, size_t y) {
    try {
        gameField[x][y]->addEntity(entity);
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

std::set<Entity*> const& Level::getMonsters() const {
    return monsters;
}

std::set<Entity *> const& Level::getOperatives() const {
    return operatives;
}

std::pair<size_t, size_t> Level::findSquare(Square* square) {
    for (int i = 0; i < gameField.getRows(); i++) {
        for (int j = 0; j < gameField.getColumns(); j++) {
            if(gameField[i][j] == square) {
                return {i, j};
            }
        }
    }
    throw std::logic_error("Not found square");
}


struct checkBarriersResult {
    Entity *entity = nullptr;
    size_t x;
    size_t y;
    bool inBorders = false;
};

checkBarriersResult checkBarriers(int viewingRadius, Directions const direction, size_t x_, size_t y_, Matrix<Square*> & field) {
    if(x_ >= field.getRows() || y_ >= field.getColumns()) {
        return {nullptr, 0, 0, false};
    }

    switch (direction) {
        case Directions::east : {
            size_t border = field.getColumns() <= y_ + viewingRadius ? field.getColumns() : y_ + viewingRadius;
            for (size_t i = y_; i < border; i++) {
                if(i != y_) {
                    if(field[x_][i]->getEntity()) {
                        return {field[x_][i]->getEntity(), x_, i, true};
                    }

                    if (field[x_][i]->getSquareType() == SquareType::Barrier || field[x_][i]->getSquareType() == SquareType::Window) {
                        return {nullptr, x_, i, true};
                    }

                    if (field[x_][i]->getSquareType() == SquareType::Wall) {
                        break;
                    }
                }
            }
           break;
        }
        case Directions::south : {
            size_t border = field.getRows() <= x_ + viewingRadius ? field.getRows() : x_ + viewingRadius;
            for (size_t i = x_; i < border; i++) {
                if (i != x_) {
                    if(field[i][y_]->getEntity()) {
                        return {field[i][y_]->getEntity(), i, y_, true};
                    }

                    if (field[i][y_]->getSquareType() == SquareType::Barrier || field[i][y_]->getSquareType() == SquareType::Window) {
                        return {nullptr, i, y_, true};
                    }

                    if (field[i][y_]->getSquareType() == SquareType::Wall) {
                        break;
                    }
                }
            }
            break;
        }
        case Directions::west : {
            size_t border = y_ > viewingRadius ? y_ - viewingRadius : 0;
            for (size_t i = y_ + 1; i > border; i--) {
                if(i != y_+ 1) {
                    if(field[x_][i - 1]->getEntity()) {
                        return {field[x_][i - 1]->getEntity(), x_, i - 1, true};
                    }

                    if (field[x_][i - 1]->getSquareType() == SquareType::Barrier || field[x_][i - 1]->getSquareType() == SquareType::Window) {
                        return {nullptr, x_, i - 1, true};
                    }

                    if (field[x_][i - 1]->getSquareType() == SquareType::Wall) {
                        break;
                    }
                }
            }
            break;
        }
        case Directions::north : {
            size_t border = x_ > viewingRadius ? x_ - viewingRadius : 0;
            for (size_t i = x_ + 1; i > border; i--) {
                if(i != x_ + 1) {
                    if(field[i - 1][y_]->getEntity()) {
                        return {field[i - 1][y_]->getEntity(), i - 1, y_, true};
                    }

                    if (field[i - 1][y_]->getSquareType() == SquareType::Barrier || field[i - 1][y_]->getSquareType() == SquareType::Window) {
                        return {nullptr, i - 1, y_, true};
                    }

                    if (field[i - 1][y_]->getSquareType() == SquareType::Wall) {
                        break;
                    }
                }
            }
            break;
        }
    }
    return {nullptr, 0 , 0 , false};
}

Entity* findNeighborhoodVictim(Directions const direction, size_t x_, size_t y_, Matrix<Square*> & field) {
    Entity * victim = nullptr;
    switch (direction) {
        case Directions::north : {
            if(x_ == 0) { throw std::logic_error("first row and north"); }
            victim = field[x_ - 1][y_]->getEntity();
            break;
        }
        case Directions::east : {
            if(y_ == field.getColumns() - 1) {throw std::logic_error("end column and east");}
            victim = field[x_][y_ + 1]->getEntity();
            break;
        }
        case Directions::south : {
            if(x_ == field.getRows() - 1) {throw std::logic_error("end row and south"); }
            victim = field[x_ + 1][y_]->getEntity();
            break;
        }
        case Directions::west : {
            if(y_ == 0) {throw std::logic_error("first column and west"); }
            victim = field[x_ - 1][y_]->getEntity();
            break;
        }
    }
    return victim;
}

AttackService::AttackService(std::shared_ptr<GameService> const& game) : gameService_(game) {}

MoveService::MoveService(std::shared_ptr<GameService> const& game) : gameService_(game) {}

bool AttackService::attack(Entity* entity, Directions const direction){

    Level & level = gameService_->getLevel();

    std::pair<size_t, size_t> entityPos;
    try {
        entityPos = findPos(entity, level.getGameField());
    } catch (std::exception &) {
        return false;
    }

    Entity* victim = nullptr;
    checkBarriersResult res;

    if(dynamic_cast<SmartEntity*>(entity)) {
        res = checkBarriers(entity->getViewingRadius(), direction, entityPos.first, entityPos.second, level.getGameField());
        victim = res.entity;
    } else {
        victim = findNeighborhoodVictim(direction, entityPos.first, entityPos.second, level.getGameField());
    }

    if((dynamic_cast<Operative*>(victim) && dynamic_cast<Operative*>(entity)) ||
        (!dynamic_cast<Operative*>(victim) && !dynamic_cast<Operative*>(entity))) {
        return false;
    }

    if(victim == nullptr && res.inBorders == false) {
        return false;
    }

    auto * attackingEntity = dynamic_cast<Attacking*>(entity);

    int damage = attackingEntity->attack();

    bool breakBorders = false;
    if(res.inBorders == true && res.entity == nullptr && damage) {
        level.getGameField()[res.x][res.y]->changeSquareType(SquareType::Floor);
        breakBorders = true;
    }

    if(victim == nullptr) {
        return false;
    }

    entityPos = findPos(victim, level.getGameField());

    if(!breakBorders) {
        if(victim->getCurrentHealth() <= damage) {
            auto victimItem = dynamic_cast<ActionItem*>(victim);
            if(victimItem) {
                level.addItemToSquare(victimItem->throwAllItems(), entityPos.first, entityPos.second);
            }
            level.deleteEntity(victim);
        } else {
            victim->setHealth(victim->getCurrentHealth() - damage);
        }
    }

    return true;
}

/*
    A* algorithm
 */

bool checkSquare(size_t x, size_t y, Matrix<Square*> & field, std::initializer_list<SquareType> const acceptTypes) {
    if(x >= field.size().first || y >= field.size().second) return false;
    for (auto const& i : acceptTypes) {
        if(field[x][y]->getSquareType() == i) {
            return true;
        }
    }
    return false;
}

bool checkSquareWithEntities(size_t x, size_t y, Matrix<Square*> & field, std::initializer_list<SquareType> const acceptTypes) {
    if(x >= field.size().first || y >= field.size().second) return false;
    for (auto const& i : acceptTypes) {
        if(field[x][y]->getSquareType() == i) {
            if(field[x][y]->getEntity()) {
                return false;
            }
            return true;
        }
    }
    return false;
}

bool checkSquare(std::pair<size_t, size_t> size_, Matrix<Square> & field, std::initializer_list<SquareType> const acceptTypes) {
    if(size_.first >= field.size().first || size_.second >= field.size().second) return false;
    for (auto const& i : acceptTypes) {
        if(field[size_.first][field.size().second].getSquareType() == i) {
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

std::vector<Square *> findMinWay(size_t x1, size_t y1, size_t x2, size_t y2, Level & level) {

    if(!checkSquare(x1, y1, level.getGameField(), {SquareType::Storage, SquareType::Floor}) ||
        !checkSquare(x2, y2, level.getGameField(), {SquareType::Storage, SquareType::Floor})) {
        throw std::logic_error("invalid coordinates");
        }

    auto cmp = [](elementQueue & elem1, elementQueue & elem2) {return elem1.grade > elem2.grade;};

    std::priority_queue<elementQueue, std::vector<elementQueue>, decltype(cmp)> queue(cmp);

    elementQueue start{0, long(x1), long(y1)};
    elementQueue finish{0, long(x2), long(y2)};
    Matrix<std::pair<size_t, size_t>> prev(level.size().first, level.size().second);
    Matrix<long> weights(level.size().first, level.size().second, LONG_MAX);
    std::unordered_set<Square*> visited;

    queue.emplace(start);
    weights[start.x][start.y] = 0;

    while (!queue.empty()) {
        elementQueue work = queue.top();

        if(work == finish) {
            break;
        }
        if(checkSquare(work.x, work.y - 1, level.getGameField(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x][work.y - 1] > work.grade + heuristic(work.x, work.y - 1, finish) + 1) {
                weights[work.x][work.y - 1] = work.grade + heuristic(work.x, work.y - 1, finish) + 1;
                prev[work.x][work.y - 1] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x, work.y - 1, finish) + 1, work.x, work.y - 1});
            }
        }

        if(checkSquare(work.x, work.y + 1, level.getGameField(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x][work.y + 1] > work.grade + heuristic(work.x, work.y + 1, finish) + 1) {
                weights[work.x][work.y + 1] = work.grade + heuristic(work.x, work.y + 1, finish) + 1;
                prev[work.x][work.y + 1] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x, work.y + 1, finish) + 1, work.x, work.y + 1});
            }
        }

        if(checkSquare(work.x - 1, work.y, level.getGameField(), {SquareType::Storage, SquareType::Floor})) {
            if(weights[work.x - 1][work.y] > work.grade + heuristic(work.x - 1, work.y, finish) + 1) {
                weights[work.x - 1][work.y] = work.grade + heuristic(work.x - 1, work.y, finish) + 1;
                prev[work.x - 1][work.y] = {work.x, work.y};
                queue.push({work.grade + heuristic(work.x - 1, work.y, finish) + 1, work.x - 1, work.y});
            }
        }

        if(checkSquare(work.x + 1, work.y, level.getGameField(), {SquareType::Storage, SquareType::Floor})) {
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
        result.push_back(level.getGameField()[i.x][i.y]);
        swap = i;
        i.x = prev[swap.x][swap.y].first;
        i.y = prev[swap.x][swap.y].second;
    }
    result.push_back(level.getGameField()[start.x][start.y]);

    return result;
}

bool MoveService::move(Entity* entity, Directions direction) {

    Level & level = gameService_->getLevel();

    std::pair<size_t, size_t> entityPos;
    entityPos = findPos(entity, level.getGameField());

    switch (direction) {
        case Directions::north : {
            if(entityPos.first == 0) { throw std::logic_error("first row and north"); }
            if(level.getGameField()[entityPos.first - 1][entityPos.second]->getEntity() == nullptr) {
                level.getGameField()[entityPos.first][entityPos.second]->deleteEntity();
                level.getGameField()[entityPos.first - 1][entityPos.second]->addEntity(entity);
            }
            break;
        }
        case Directions::east : {
            if(entityPos.second == level.getGameField().size().second - 1) {throw std::logic_error("end column and east");}
            if(level.getGameField()[entityPos.first][entityPos.second + 1]->getEntity() == nullptr) {
                level.getGameField()[entityPos.first][entityPos.second]->deleteEntity();
                level.getGameField()[entityPos.first][entityPos.second + 1]->addEntity(entity);
            }
            break;
        }
        case Directions::south : {
            if(entityPos.first == level.getGameField().size().first - 1) {throw std::logic_error("end row and south"); }
            if(level.getGameField()[entityPos.first + 1][entityPos.second]->getEntity() == nullptr) {
                level.getGameField()[entityPos.first][entityPos.second]->deleteEntity();
                level.getGameField()[entityPos.first + 1][entityPos.second]->addEntity(entity);
            }
            break;
        }
        case Directions::west : {
            if(entityPos.second == 0) {throw std::logic_error("first column and west"); }
            if(level.getGameField()[entityPos.first][entityPos.second - 1]->getEntity() == nullptr) {
                level.getGameField()[entityPos.first][entityPos.second]->deleteEntity();
                level.getGameField()[entityPos.first][entityPos.second - 1]->addEntity(entity);
            }
            break;
        }
    }
    entity->move();
    return true;
}

std::vector<std::pair<size_t, size_t>> findStorages(Matrix<Square*> & field) {
    std::vector<std::pair<size_t, size_t>> result;
    for (int i = 0; i < field.size().first; i++) {
        for (int j = 0; j < field.size().second; j++) {
            if(field[i][j]->getSquareType() == SquareType::Storage) {
                result.push_back({i, j});
            }
        }
    }
    return result;
}

// Функция для проверки того, видно ли объект (последняя точка не учитывается)

bool viewingObjectArea(int start_x, int start_y, int target_x, int target_y, Matrix<Square*> & field) {
    int x = start_x, y = start_y;
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

std::vector<Entity*> entityScanerRadius(Entity * entity, int x, int y, Matrix<Square*> & field) {
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
                if(field[i][j]->getEntity() && !(i == x && j == y)) {
                    if(viewingObjectArea(x, y, i, j, field)) {
                        result.push_back(field[i][j]->getEntity());
                    }
                }
            }
        }
    }

    return result;
}

/*
 Приоритеты AI для entity
 + Перечень целей для монстров

[v] 1. Монстры с ближней атакой ходят и ищут жертв - рандомно, но назад не ходят
    (если зажаты среди клеток - правило нарушается и настурают на любую клетку)
2. Те, у кого есть оружие и есть в радиусе оперативники
    a. Дойти до ближайшей точки для совершения атаки выстрела
        I. Если по пути в клетке есть оружие - взять рандомно, если есть патроны (да/нет);
    b. Атаковать, пока есть возможность (временная, запасы оружия)
    c. Если осталось время на атаку - добавить цель принести
3. Те, у кого нет оружия, но может быть - добавить цель для поиска фуражирами
4. Поиск путей и возможностей подноса оружия
    PRIORITY: Поиск фуражира с оружием и ближайший с возможностью донести выбирается, иначе - п. c
    a. Выбирается тот, кто может дойти до ближайшего хранилища с большим инвентарем
        I. Если нужное количество оружия найдено и вместилось в инвентарь - перейти к п. b
        II. Если нет места - повторить для следующего персонажа по вместимости персонажа
        III. Если не хватает времени для перемещения к атакующему - перейти к п. c
    b. Дойти до ближайшей цели и разносить оружие, пока не кончатся очки времени, если очки времени закончились - перейти к п. c
    c. Определить количество клеток, необходимое для доноса оружия - построить маршрут с "пересадками" предметов
5. Нет задач и остались очки времени - рандомное перемещение и поиск предметов
 */

EntityAI::EntityAI(std::shared_ptr<GameService> const& game) : AttackService(game), MoveService(game) {}

bool MoveService::findAttackingNoSmart(Entity* entity) {

    Level & level = gameService_->getLevel();

    Matrix<bool> visited(level.size().first, level.size().second, false);
    std::pair<size_t, size_t> coordinates = findPos(entity, level.getGameField());

    while (entity->getCurrentTime() > entity->getMoveTime()) {
        std::vector<char> aroundSquares(4); //north, east, south, west
        std::vector<Entity*> scan = entityScanerRadius(entity, coordinates.first, coordinates.second, level.getGameField());
        for (auto i : scan) {
            if(!(level.getMonsters().count(entity) && level.getMonsters().count(i)) &&
                !(level.getOperatives().count(entity) && level.getOperatives().count(i))) {
                std::pair<size_t, size_t> victimCoordinates = findPos(i, level.getGameField());
                std::vector<Square*> path = findMinWay(coordinates.first, coordinates.second,
                    victimCoordinates.first, victimCoordinates.second, level);
                if(path.size() <= 2) {
                    return true;
                }
                if(i->getMoveTime() * (path.size() - 2) < i->getCurrentTime()) {
                    deleteEntityFromField(entity, level.getGameField());
                    std::pair<size_t, size_t> victimPos = level.findSquare(path[path.size() - 1]);
                    level.addEntity(entity, victimPos.first, victimPos.second);
                    entity->setTime(entity->getCurrentTime() - entity->getMoveTime() * (path.size() - 2));
                    return true;
                }
            }
        }

        if(checkSquareWithEntities(coordinates.first - 1, coordinates.second, level.getGameField(),
        {SquareType::Floor, SquareType::Storage})) {
            aroundSquares[0] = visited[coordinates.first - 1][coordinates.second] ? 1 : 0;
        } else {
            aroundSquares[0] = 2;
        }

        if(checkSquareWithEntities(coordinates.first, coordinates.second + 1, level.getGameField(),
        {SquareType::Floor, SquareType::Storage})) {
            aroundSquares[1] = visited[coordinates.first][coordinates.second + 1] ? 1 : 0;
        } else {
            aroundSquares[1] = 2;
        }

        if(checkSquareWithEntities(coordinates.first + 1, coordinates.second, level.getGameField(),
        {SquareType::Floor, SquareType::Storage})) {
            aroundSquares[2] = visited[coordinates.first + 1][coordinates.second] ? 1 : 0;
        } else {
            aroundSquares[2] = 2;
        }

        if(checkSquareWithEntities(coordinates.first, coordinates.second - 1, level.getGameField(),
        {SquareType::Floor, SquareType::Storage})) {
            aroundSquares[3] = visited[coordinates.first][coordinates.second - 1] ? 1 : 0;
        } else {
            aroundSquares[3] = 2;
        }

        size_t countFreeSquares = 0;
        size_t countBarriers = 0;
        countFreeSquares = std::count(aroundSquares.begin(), aroundSquares.end(), 0);
        countBarriers = std::count(aroundSquares.begin(), aroundSquares.end(), 2);

        int direction = 0;
        if(countFreeSquares) {
            int randomDirection = rand() % countFreeSquares;
            int counter = -1;
            for (int i = 0; i < aroundSquares.size(); i++) {
                if(counter < randomDirection && aroundSquares[i] == 0) { counter++; }
                if(counter == randomDirection) {
                    direction = i;
                    break;
                }
            }
        } else if(countBarriers != 4) {
            int randomDirection = rand() % (4 - countBarriers);
            int counter = -1;
            for (int i = 0; i < aroundSquares.size(); i++) {
                if(counter < randomDirection && (aroundSquares[i] == 1 || aroundSquares[i] == 0)) { counter++; }
                if(counter == randomDirection) {
                    direction = i;
                    break;
                }
            }
        } else {
            return false;
        }

        visited[coordinates.first][coordinates.second] = true;
        switch (direction) {
            case 0 : {move(entity, Directions::north); coordinates.first--; break; }
            case 1 : {move(entity, Directions::east); coordinates.second++; break; }
            case 2 : {move(entity, Directions::south); coordinates.first++; break; }
            case 3 : {move(entity, Directions::west); coordinates.second--; break; }
            default : break;
        }
    }
    return true;
}

Directions findEntityAround(Entity* entity, Matrix<Square*> & gameField) {
    std::pair<size_t, size_t> entityPos = findPos(entity, gameField);
    Entity *res = nullptr;
    std::vector directions = {Directions::north, Directions::east, Directions::south, Directions::west};
    for (auto i : directions) {
        try {
            res = findNeighborhoodVictim(i, entityPos.first, entityPos.second, gameField);
            if(res) {
                return i;
            }
        } catch (std::exception const&) {}
    }
    throw std::runtime_error("No entities around");
}

void MoveService::walkingNearestAttackingEntities() {
    Level & level = gameService_->getLevel();

    for (auto & i : level.getMonsters()) {
        if(!dynamic_cast<SmartEntity*>(i)) {
            if(findAttackingNoSmart(i)) {
                try {
                    Directions dir = findEntityAround(i, level.getGameField());

                } catch (std::exception const&) {}
            }
        }
    }
}

void EntityAI::AITick() {
    Level & level = AttackService::gameService_->getLevel();

    std::vector<std::pair<size_t, size_t>> storagePoints = findStorages(level.getGameField());
    std::vector<bool> visitedStorages(storagePoints.size());

    walkingNearestAttackingEntities();

}


