#include <iostream>

class Game {
    private:
    int score = 0;
    int suply = 10;

    public:
    std::string CMD;
    std::string help;

    void errorHandel (std::string& CMD) { std::cout << "There is no command called " << CMD << "\n"; }

    void buySupply () {int su;std::cout << "Suply to buy: "; std::cin >> su;score -= su; suply += su;}
    
    void quit (bool& run) { run = false;}

    void log () {std::cout << " SCORE: " << score << "\n" << " SUPLY: " << suply << "\n";}

    void step () {score += 2; suply -= 1;std::cout << "STEP \n";}

    void loop (bool& run) {
        std::cin >> CMD;
        if (CMD == "log") { log(); }
        else if (CMD == "step") { step(); }
        else if (CMD == "suply") { buySupply(); }
        else if (CMD == "quit") { quit(run); 
        } else { errorHandel(CMD); }
    }

};


int main () {
    bool run = true;
    Game game;

    while (run) {
        game.loop(run);
    }

    return 0;
}