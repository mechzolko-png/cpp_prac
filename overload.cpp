#include <iostream>
#include <string>


void show (std::string text) {
    std::cout << "\n" << text;
};

void show (double PI) {
    std::cout << "\n" << PI;
};

void show (int num) {
    std::cout << "\n" << num;
};


int main () {
    int help;
    int a = 10;
    double b = 3.14;
    std::string c = "Hello World!";

    show(a);
    show(b);
    show(c);

    std::cin >> help;

    return 0;
};