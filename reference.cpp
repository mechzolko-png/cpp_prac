#include <iostream>

void change (int& x) {
    x = 100;
}

int main () {
    int number = 10;
    int help;

    std::cout << "Before: " << number << "\n";

    change(number);

    std::cout << "After: " << number << "\n";

    std::cin >> help; 

    return 0;
};