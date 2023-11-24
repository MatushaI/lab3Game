#include <iostream>
#include "game_lib/gameLib.hpp"
#include <string>
#include <vector>
#include <thread>

int main() {
    std::vector<std::string> aboba = {"name1", "name2"};
    Weapon *test = new Weapon("testGun", "MachineGun");
    CartridgeContainer * test2 = new CartridgeContainer("conter", "12,7mm", 100, 0, 20);

    MedKit * med = new MedKit("med", 100, 1000);
    Operative smart("aboba", 100, 100, 5, 3);
    std::cout << smart.addItem(test);
    std::cout << smart.addItem(med);
    smart.setHealth(50);
    std::cout << std::endl << smart.getCurrentHealth() << std::endl;
    std::cout << smart.useMedKit().first;
    std::cout << std::endl << smart.getCurrentHealth() << std::endl;
    //std::cout << smart.throwItem("med")->getName();


    Item * testItem = smart.throwItem("");
    //std::cout << smart.canKeeping(test2);

    AttackService serv;
    serv.gameService->getLevel().addEntity(&smart, 1, 1);
    //std::cout << serv.gameService->getLevel().getGameField()[1][1].getEntity() << " " << &smart;

    serv.attack(&smart, Directions::east);

    //Inventory invent(1, 4);
    //Weapon test1("MachineGun", 25500, 50, 1, 10, "alabugaTech", 300);

    return 0;
}
