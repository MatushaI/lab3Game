#include <iostream>
#include <queue>

#include "items.h"
#include "enemy.h"
#include "gameServices.h"
#include <string>
#include <vector>
#include <thread>


int main() {

    auto * oper = new Furajire("aboba", 40, 40, 40, 20, 10, 10);
    //std::cout << oper->getName();
    GameService * game = new GameService();
    auto * move = new MoveService(game);



    for (int i = 0; i < 9; i++) {
        move->gameService->getLevel().getGameField()[i][4].changeSquareType(SquareType::Wall);
    }
    move->gameService->getLevel().setSize(100, 100);
    move->gameService->getLevel().setSize(10, 10);

    move->gameService->getLevel().getGameField()[3][9].changeSquareType(SquareType::Wall);
    std::vector<Square*> res = move->findMinWay(0, 0, 99, 99);

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

    return 0;
}


