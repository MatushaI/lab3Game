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

    mainwindow * mainView = new mainwindow();
    mainView->resize(500, 500);
    mainView->setWindowTitle("hui");
    mainView->show();



    //auto * oper = new Furajire("aboba", 40, 40, 40, 20, 10, 10);
    //std::cout << oper->getName();
    auto * game = new GameService();
    auto * move = new MoveService(game);

    auto * gun = new Weapon("boooom", "AK-47");
    auto * wildGun = new Weapon("boooom2", "MachineGun");

    for (int i = 0; i < 9; i++) {
        move->gameService->getLevel().getGameField()[i][4].changeSquareType(SquareType::Wall);
    }
    //move->gameService->getLevel().setSize(100, 100);
    move->gameService->getLevel().setSize(10, 10);
    move->gameService->getLevel().getGameField()[6][5].changeSquareType(SquareType::Floor);

    auto * wildAboba = new SmartEntity("chushka", 100, 100, 30, 30, 100, 0.5);
    auto * oper = new Operative("Churka", 31, 100, 30, 30, 100, 0.5);

    gun->setCurrentCartridges(50);
    oper->addActiveItem(gun);
    wildGun->setCurrentCartridges(50);
    wildAboba->addItem(wildGun);

    auto * attack_service = new AttackService(game);
    game->getLevel().addEntity(wildAboba, 4, 5);
    game->getLevel().addEntity(oper, 8, 5);

    entityScanerRadius(wildAboba, 0, 0, game->getLevel().getGameField());

    std::cout << wildAboba->getCurrentHealth() << std::endl;
    attack_service->attack(oper, Directions::south);
    attack_service->attack(oper, Directions::south);
    attack_service->attack(oper, Directions::south);
    std::cout << wildAboba->getCurrentHealth() << std::endl;
    std::cout << move->gameService->getLevel().getGameField()[4][5].getItems()[0]->getName();

    move->gameService->getLevel().getGameField()[3][9].changeSquareType(SquareType::Wall);
    //move->gameService->getLevel().getGameField()[3][2].changeSquareType(SquareType::Wall);
    std::vector<Square*> res = move->findMinWay(0, 0, 9, 9);

    //std::cout << viewingObjectArea(0, 3, 3, 0, game->getLevel().getGameField()) << std::endl;
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
    return app->exec();
    return 0;
}


