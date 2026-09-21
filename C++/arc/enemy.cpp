#include "player.h"
#include "enemy.h"

     
void Enemy::attack (Player* player) {
    player->TakeDamage(damage);
}