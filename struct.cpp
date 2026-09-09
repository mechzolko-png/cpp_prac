#include <iostream>
#include <string>


struct Player {
    std::string name;
    int age;
    int score;

    void getScore(int a) {
        score += a;
    }
};

struct Enemy {
    std::string name;
    int age;
    int score;
};


int main () {
    Player player;
    Enemy enemy;

    player.name = "Gabi";
    player.age = 18;
    player.score = 0;

    enemy.name = "Devil";
    enemy.age = 21500;
    enemy.score = 100;
  
    return 0;
}