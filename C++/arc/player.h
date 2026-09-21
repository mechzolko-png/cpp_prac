#include <string>
using namespace std;
#pragma once

class Player {
    public:
    string name;
    int hp;

    Player (int hp, string name);

    void TakeDamage (int damage);

    void printState ();
};