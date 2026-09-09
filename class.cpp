#include <iostream>
#include <string>


class Player {
    std::string name;
    int score;

    public:
    Player (std::string pname, int pscore) {
        name = pname;
        score = pscore;
    }
};


class Test {
    public:
    Test() { // constructor (same name as the class)
        std::cout << "Player created";
    }

    ~Test () { // desctructor
        std::cout << "Player destoryed";
    }
};


class BankAccount {
    public:  // can manipulted/used by main 
    void deposit(int dep) {
        if (dep > 0) {
            balance += dep;
        }
    }

    private:    // can't been manipulated by main
    int balance = 1000;

};


int main () {
    BankAccount banka;
    Player player("Zach",100);
    Player player2("Yankab",50);

    banka.deposit(100);

    return 0;
}