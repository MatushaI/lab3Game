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
    QApplication *app = new QApplication(argc, nullptr);

    auto * game = new GameService();
    auto * move = new MoveService(game);

    //auto * oper = new Furajire("aboba", 40, 40, 40, 20, 10, 10);
    //std::cout << oper->getName();

    auto * gun = new Weapon("boooom", "AK-47");
    auto * wildGun = new Weapon("boooom2", "MachineGun");

    for (int i = 0; i < 9; i++) {
        move->gameService->getLevel().getGameField()[i][4]->changeSquareType(SquareType::Barrier);
    }
    for (int i = 4; i < 15; i++) {
        move->gameService->getLevel().getGameField()[10][i]->changeSquareType(SquareType::Wall);
    }
    //move->gameService->getLevel().setSize(100, 100);
    //move->gameService->getLevel().setSize(100, 100);
    move->gameService->getLevel().getGameField()[6][5]->changeSquareType(SquareType::Floor);
    move->gameService->getLevel().getGameField()[7][8]->changeSquareType(SquareType::Window);
    move->gameService->getLevel().getGameField()[6][9]->changeSquareType(SquareType::Barrier);
    move->gameService->getLevel().getGameField()[1][2]->changeSquareType(SquareType::Storage);

    auto * wildAboba = new wildEntity("chushka", 100, 10000, 3, 30, 100, 0.5, 20);
    auto * oper = new Operative("Churka", 31, 1000, 30, 30, 100, 0.5);

    gun->setCurrentCartridges(1);
    oper->addActiveItem(gun);
    wildGun->setCurrentCartridges(50);
    //wildAboba->addItem(wildGun);

    EntityAI ai(game, move);


    auto * attack_service = new AttackService(game);
    game->getLevel().addEntity(wildAboba, 4, 5);
    game->getLevel().addEntity(oper, 6, 5);
    ai.AITick();
    entityScanerRadius(wildAboba, 0, 0, game->getLevel().getGameField());

    std::cout << wildAboba->getCurrentHealth() << std::endl;
    std::cout << wildAboba->getCurrentTime() << std::endl;
    //attack_service->attack(oper, Directions::east);
    //attack_service->attack(oper, Directions::west);
    attack_service->attack(oper, Directions::north);
    std::cout << wildAboba->getCurrentHealth() << std::endl;
    //std::cout << move->gameService->getLevel().getGameField()[4][5].getItems()[0]->getName();

    auto * mainView = new mainwindow(nullptr, game);
    mainView->resize(1000, 1000);
    mainView->show();
    mainView->drawMap();
    move->gameService->getLevel().getGameField()[3][9]->changeSquareType(SquareType::Wall);
    //move->gameService->getLevel().getGameField()[3][2].changeSquareType(SquareType::Wall);
    std::vector<Square*> res = move->findMinWay(0, 0, 9, 9);

    //std::cout << viewingObjectArea(0, 3, 3, 0, game->getLevel().getGameField()) << std::endl;

    return app->exec();

    //return app->exec();
    return 0;
}


