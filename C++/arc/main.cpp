#include <iostream>
#include "player.h"
#include "enemy.h"

int main () {
    int help;
    Player* player = new Player(100,"ZACHARI");
    Enemy* enemy = new Enemy;
    enemy->attack(player);
    std::cin >> help;


    delete enemy;
    delete player;
    return 0;
}