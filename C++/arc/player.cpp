#include <iostream>
#include "player.h"
using namespace std;

Player::Player (int hp, string name): hp(hp), name(name) {}

void Player::TakeDamage (int damage) {
    hp -= damage;
    cout << "TAKEN DAMAGE NEW HP: " << hp;
}

void Player::printState () {
    cout << "NAME: " << name << "HP: " << hp << "\n";
}