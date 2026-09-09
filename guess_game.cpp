#include <iostream>
#include <string>


int main () {
    int secret = 37;
    int prompt;
    int help;

    bool run = true;

    std::cout << "------------ Guess The Number Game ------------" << "\n";

    while (run) {
        std::cout << "\n" << "Enter your guess> "; std::cin >> prompt; // guess input

        // game logic
        if (prompt >= secret) {
            if (prompt == secret) {
                std::cout << "\n" << "You win.";
                run = false;
            } else if (prompt != secret) {
                std::cout << "\n" << "Try lower.";
            } else {
                std::cout << "\n" << "You should write number.";
            }
        } else if (prompt < secret) {
            std::cout << "\n" << "Try bigger.";
        }

    };



    
    std::cin >> help;
    return 0;
}