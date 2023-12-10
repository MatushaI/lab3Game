#include <iostream>
#include "items.h"
#include "enemy.h"
#include "gameServices.h"
#include <string>
#include <vector>

int main() {

    //auto * oper = new Furajire("aboba", 40, 40, 40, 20, 10, 10);
    //std::cout << oper->getName();
    auto * game = new GameService();
    auto * move = new MoveService(game);

    auto * gun = new Weapon("boooom", "MachineGun");

    for (int i = 0; i < 9; i++) {
        move->gameService->getLevel().getGameField()[i][4].changeSquareType(SquareType::Wall);
    }
    //move->gameService->getLevel().setSize(100, 100);
    move->gameService->getLevel().setSize(10, 10);

    auto * wildAboba = new wildEntity("chushka", 100, 100, 100, 200, 10, 0.5, 30);
    auto * oper = new Operative("Churka", 31, 100, 30, 30, 100, 0.5);

    gun->setCurrentCartridges(50);
    oper->addActiveItem(gun);

    auto * attack_service = new AttackService(game);
    game->getLevel().addEntity(wildAboba, 4, 5);
    game->getLevel().addEntity(oper, 5, 5);

    entityScanerRadius(wildAboba, 0, 0, game->getLevel().getGameField());

    std::cout << wildAboba->getCurrentHealth() << std::endl;
    attack_service->attack(oper, Directions::north);
    std::cout << wildAboba->getCurrentHealth() << std::endl;

    move->gameService->getLevel().getGameField()[3][9].changeSquareType(SquareType::Wall);
    //move->gameService->getLevel().getGameField()[3][2].changeSquareType(SquareType::Wall);
    std::vector<Square*> res = move->findMinWay(0, 0, 9, 9);

    std::cout << viewingObjectArea(0, 3, 3, 0, game->getLevel().getGameField()) << std::endl;
/*
    for (int i = 0; i < move->gameService->getLevel().size().first; i++) {
        for (int j = 0; j < move->gameService->getLevel().size().second; ++j) {
            auto l = std::find(res.begin(), res.end(), &move->gameService->getLevel().getGameField()[i][j]);
            if(l != res.end()) {
                std::cout << "🟩";
            } else if(move->gameService->getLevel().getGameField()[i][j].getSquareType() == SquareType::Wall) {
                std::cout << "⬛";
            } else {
                std::cout << "⬜";
            }
        }
        std::cout << "\n";
    }
*/
    return 0;
}


