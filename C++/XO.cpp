#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

char* pMap;
int* pWins;

class Game {
    private:
    char map[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    pMap = &map;
    int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8},
        {0,3,6}, {1,4,7}, {2,5,8},
        {0,4,8}, {2,4,6}
    };
    pWins = &wins;

    public:
    void drawMap () {
        cout << "      __________________ \n";
        cout << "     |     |     |      |\n";
        cout << "     |  "<< map[0] << "  |" << "  "<< map[1] << "  |" << "  "<< map[2] << "   |\n";
        cout << "     |_____|_____|______|\n";
        cout << "     |     |     |      |\n";
        cout << "     |  "<< map[3] << "  |" << "  "<< map[4] << "  |" << "  "<< map[5] << "   |\n";
        cout << "     |_____|_____|______|\n";
        cout << "     |     |     |      |\n";
        cout << "     |  "<< map[6] << "  |" << "  "<< map[7] << "  |" << "  "<< map[8] << "   |\n";
        cout << "     |_____|_____|______|\n";
    }  

    void putX () {
        int place;
        while (true) {
            cout << "Where do you want to put an X ? (1-9): ";
            cin >> place;
            if (map[place - 1] == ' ') {map[place - 1] = 'X';break;}
            else if (map[place - 1] != ' ') { cout << "Chose somewhere else(1-9): "; cin >> place;}
            else {cout << "Chose somewhere else(1-9): "; cin >> place;}
        }
    }

    void putO () {
        
        while (true) {
            int numb = rand() % 9;
            if (map[numb] == ' ') { map[numb] = 'O'; break;}
        }
    }

    bool check() {
        // PLAYER WINS
        if (map[0] == 'X' && map[1] == 'X' && map[2] == 'X') { cout << "Player win!"; return false; }
        if (map[3] == 'X' && map[4] == 'X' && map[5] == 'X') { cout << "Player win!"; return false; }
        if (map[6] == 'X' && map[7] == 'X' && map[8] == 'X') { cout << "Player win!"; return false; }
        if (map[0] == 'X' && map[3] == 'X' && map[6] == 'X') { cout << "Player win!"; return false; }
        if (map[1] == 'X' && map[4] == 'X' && map[7] == 'X') { cout << "Player win!"; return false; }
        if (map[2] == 'X' && map[5] == 'X' && map[8] == 'X') { cout << "Player win!"; return false; }
        if (map[0] == 'X' && map[4] == 'X' && map[8] == 'X') { cout << "Player win!"; return false; }
        if (map[2] == 'X' && map[4] == 'X' && map[6] == 'X') { cout << "Player win!"; return false; }

        // BOT WINS
        if (map[0] == 'O' && map[1] == 'O' && map[2] == 'O') { cout << "Bot win!"; return false; }
        if (map[3] == 'O' && map[4] == 'O' && map[5] == 'O') { cout << "Bot win!"; return false; }
        if (map[6] == 'O' && map[7] == 'O' && map[8] == 'O') { cout << "Bot win!"; return false; }
        if (map[0] == 'O' && map[3] == 'O' && map[6] == 'O') { cout << "Bot win!"; return false; }
        if (map[1] == 'O' && map[4] == 'O' && map[7] == 'O') { cout << "Bot win!"; return false; }
        if (map[2] == 'O' && map[5] == 'O' && map[8] == 'O') { cout << "Bot win!"; return false; }
        if (map[0] == 'O' && map[4] == 'O' && map[8] == 'O') { cout << "Bot win!"; return false; }
        if (map[2] == 'O' && map[4] == 'O' && map[6] == 'O') { cout << "Bot win!"; return false; }

        // TIE GAME
        if (map[0] != ' ' && map[1] != ' ' && map[2] != ' ' && map[3] != ' ' && map[4] != ' ' && map[5] != ' ' && map[6] != ' ' && map[7] != ' ' && map[8] != ' ') { cout << "TIE game"; return false; }

        return true;
    }

};


class AI {
    private:
    
    public:

    int hasWinStep () {
        for (int i = 0; i < 8; i++;) {
            int bot = 0;
            for (int j = 0; j < 3; j++;) {
                if (*pWins[i][j] == 'O') {bot++;}
            }
            if (bot == 2) {
                for (int j = 0; j < 3; j++;) {
                    if (*pWins[i][j] != 'O' && *pWins[i][j] != ' ') {return *pWins[i][j];}
                } else {
                    return 101;
                }
            } else {
                return 101;
            }
        }
        return 101;
    }

    int isInDanger () {
        for (int i = 0; i < 8; i++;) {
            int player = 0;
            for (int j = 0; j < 3; j++;) {
                if (*pWins[i][j] == 'X') {player++;}
            }
            if (player == 2) {
                for (int j = 0; j < 3; j++;) {
                    if (*pWins[i][j] != 'O' && *pWins[i][j] != 'X') {return *pWins[i][j];}
                } else {
                    return 101;
                }
            } else {
                return 101;
            }
        }
        return 101;
    }


    void chose () {
        int winStep = hasWinStep();

        if (winStep != 101) {
            *pMap[winStep] = 'O';
        }

        else if (winStep == 101) {
            int danger = isInDanger();
            if (danger != 101) {*pMap[danger] = 'O';}
        }

        else {
            int choice = rand() % 10;
            *pMap[danger] = 'O';
        }

    }
};


int main () {
    srand(time(0));
    int help;
    Game game;
    AI ai;
    bool run = true;
    while (run) {
        game.drawMap();
        game.putX();
        ai.chose();
        run = game.check();
    }
    
    
    cin >> help;

    return 0;
}