#include <iostream>
#include "items.h"
#include "enemy.h"
#include "gameServices.h"
#include <string>
#include <vector>
#include <QApplication>
#include "mainwindow.h"

int main() {
    int argc = 0;
    auto *app = new QApplication(argc, nullptr);

    auto game = std::make_shared<GameService>();

    //auto * oper = new Furajire("aboba", 40, 40, 40, 20, 10, 10);
    //std::cout << oper->getName();

    auto * gun = new Weapon("boooom", "AK-47");
    auto * wildGun = new Weapon("boooom2", "MachineGun");

    for (int i = 0; i < 9; i++) {
        game->getLevel().getGameField()[i][4]->changeSquareType(SquareType::Barrier);
    }
    for (int i = 4; i < 15; i++) {
        game->getLevel().getGameField()[10][i]->changeSquareType(SquareType::Wall);
    }
    auto level = game->getLevel();
    //move->gameService->getLevel().setSize(100, 100);
    //move->gameService->getLevel().setSize(100, 100);
    level.getGameField()[6][5]->changeSquareType(SquareType::Floor);
    level.getGameField()[7][8]->changeSquareType(SquareType::Window);
    level.getGameField()[6][9]->changeSquareType(SquareType::Barrier);
    level.getGameField()[1][2]->changeSquareType(SquareType::Storage);

    auto * wildAboba = new SmartEntity("chushka", 100, 400, 3, 30, 100, 0.5);
    auto * oper = new Operative("Churka", 2000, 1000, 30, 30, 100, 0.5);

    gun->setCurrentCartridges(40);
    oper->addActiveItem(gun);
    wildGun->setCurrentCartridges(300);
    wildAboba->addItem(wildGun);

    game->getLevel().addEntity(wildAboba, 4, 5);
    level.getGameField()[5][5]->changeSquareType(SquareType::Window);
    //level.getGameField()[3][5]->changeSquareType(SquareType::Barrier);
    level.getGameField()[4][4]->changeSquareType(SquareType::Window);
    level.getGameField()[4][6]->changeSquareType(SquareType::Window);
    game->getLevel().addEntity(oper, 6, 5);

    entityScanerRadius(wildAboba, 0, 0, game->getLevel().getGameField());

    auto ai = EntityAI(game);
    ai.AITick();
    //std::cout << wildGun->getCurrentCartridges();
    std::cout << "A: " << oper->getCurrentHealth() << std::endl;

    std::cout << wildAboba->getCurrentHealth() << std::endl;
    std::cout << wildAboba->getCurrentTime() << std::endl;
    //attack_service->attack(oper, Directions::east);
    //attack_service->attack(oper, Directions::west);

    auto * attack = new AttackService(game);

    //attack->getLevel().getGameField()[7][5]->changeSquareType(SquareType::Window);
    attack->attack(oper, Directions::north);
    attack->attack(oper, Directions::north);
    attack->attack(oper, Directions::north);
    std::cout << wildAboba->getCurrentHealth() << std::endl;
    //std::cout << move->gameService->getLevel().getGameField()[4][5].getItems()[0]->getName();

    auto * mainView = new mainwindow(nullptr, game.get());
    mainView->resize(1000, 1000);
    mainView->show();
    mainView->drawMap();
    level.getGameField()[3][9]->changeSquareType(SquareType::Wall);
    //move->gameService->getLevel().getGameField()[3][2].changeSquareType(SquareType::Wall);
    //std::vector<Square*> res = findMinWay(0, 0, 9, 9, level);

    //std::cout << viewingObjectArea(0, 3, 3, 0, game->getLevel().getGameField()) << std::endl;

    //return app->exec();

    //return app->exec();
    return 0;
}


