#include <raylib.h>
#include <cmath>
#include <iostream>
#include <string>

// using namespace std;


struct Player {
    int counter = 0;
    int hp = 10;
    int score = 0;
    int x = 100;
    int y = 100;
    int speed = 2;
    int vx = 0;
    int vy = 0;
    int rad = 50;
    int maxspeed = 10;
    int lap = 0;

    void move () {
        x += vx;
        y += vy;
    }

    void correction () {
        if (vx > maxspeed) {
            vx = maxspeed;
        }

        if (vy > maxspeed) {
            vy = maxspeed;
        }

        if (vx < maxspeed * -1) {
            vx = maxspeed * -1;
        }

        if (vy < maxspeed * -1) {
            vy = maxspeed * -1;
        }
        
    }

    void reset () {
        counter = 0;
        hp = 10 + 5*lap;
        score = 0;
        x = 100;
        y = 100;
        speed = 2;
        vx = 0;
        vy = 0;
        rad = 50;
        maxspeed = 10;
    }
};

struct ballObject {
    int x = 400;
    int y = 400;

    int vx = 10;
    int vy = 10;

    int rad = 60;

    void move() {x += vx; y += vy;}

    void reset() {
        x = 400;
        y = 400;

        vx = 10;
        vy = 10;

        rad = 60;
    }
};
class Game {
    private:
    int gravity = 1;
    bool lasthit = 0;

    public:
    int lap = 0;
    std::string state = "NON";
    bool used10 = false;
    bool used20 = false;
    bool used30 = false;
    bool used40 = false;
    bool used50 = false;
    bool used60 = false;

    void applyCollision (int wx, int wy, int* px, int* py, int rad, int* pvx, int* pvy) {
        // cout << "POINTER INF py: " << *py << "\n";
        // cout << "POINTER INF px: " << *px << "\n";
        if (wx - rad <= *px) {*px = wx - rad;*pvx *= -1;}
        if (wy - rad <= *py) {*py = wy - rad;*pvy *= -1;}
        if (0 + rad >= *px) {*px = 0 + rad;*pvx *= -1;}
        if (0 + rad >= *py) {*py = 0 + rad;*pvy *= -1;}
    }

    void collisionToBall (ballObject* ball,int wx, int wy) {
        if (wx - ball->rad <= ball->x) {ball->x = wx - ball->rad; ball->vx *= -1;}
        if (wy - ball->rad <= ball->y) {ball->y = wy - ball->rad;ball->vy *= -1;}
        if (0 + ball->rad >= ball->x) {ball->x = 0 + ball->rad;ball->vx *= -1;}
        if (0 + ball->rad >= ball->y) {ball->y = 0 + ball->rad;ball->vy *= -1;}
    }

    void applyGravity (Player* p) {
        p->vy += gravity;
    }

    void collisionDetenction (ballObject* ball, Player* player) {
        if (abs(player->x - ball->x) < ball->rad + player->rad && abs(player->y - ball->y) < ball->rad + player->rad) {
            if (!lasthit) {
                player->hp--;
                lasthit = 1;
            }
        } else {
            lasthit = 0;
        }
    }

    void reset () {
        state = "NON";
        lasthit = 0;
    }
};



int main () {
    InitWindow(1280,720,"GAME");
    SetTargetFPS(60);
    Game game;
    Player* player = new Player;
    ballObject* ball = new ballObject;  // dangerous :| 
    int worldX = GetScreenWidth();
    int worldY = GetScreenHeight();

    // cout << "POINTER INM py: " << *py << "\n";
    // cout << "POINTER INM px: " << *px << "\n";

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_D)) {
            player->vx += player->speed;
        }
        if (IsKeyDown(KEY_A)) {
            player->vx -= player->speed;
        }
        if (IsKeyDown(KEY_W)) {
            player->vy -= player->speed;
        }
        if (IsKeyDown(KEY_S)) {
            player->vy += player->speed;
        } if (IsKeyPressed(KEY_R)) {
            game.lap++;
            player->lap++;
            ball->reset();
            player->reset();
            game.reset();
        } 

        if (IsKeyDown(KEY_Q) && IsKeyDown(KEY_SPACE) && IsKeyDown(KEY_LEFT_CONTROL)) {
            player->hp *= 2;
        }

        game.applyGravity(player);
        player->correction();
        ball->move();
        player->move();


        game.applyCollision(worldX,worldY,&player->x,&player->y,player->rad,&player->vx,&player->vy);
        game.collisionToBall(ball,worldX,worldY);
        game.collisionDetenction(ball,player);

        float dt = GetFrameTime();


        if (game.state == "NON") {player->counter++;}
        if (player->counter == 60) {player->score++;player->counter = 0;}

        if (player->score == 65) {game.state = "WIN";}
        if (player->score == 10 && !game.used10) {ball->vx *= 2;ball->vy *= 2; game.used10 = true; player->maxspeed *= 2;}
        if (player->score == 20 && !game.used20) {ball->vx *= 2;ball->vy *= 0.5; game.used20 = true; player->maxspeed *= 2;}
        if (player->score == 30 && !game.used30) {ball->vx *= 0.5;ball->vy *= 2; game.used30 = true; player->maxspeed *= 2;}
        if (player->score == 40 && !game.used40) {ball->vx *= 2;ball->vy *= 2; game.used40 = true; player->maxspeed *= 2;}
        if (player->score == 50 && !game.used50) {ball->vx *= 2;ball->vy *= 2; game.used50 = true; player->maxspeed *= 2;}
        if (player->score == 60 && !game.used60) {ball->vx *= 5;ball->vy *= 2; game.used60 = true; player->maxspeed *= 2;}
        if (player->hp == 0) {game.state = "LOST";}
    
        if (game.state == "NON") {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawCircle(player->x,player->y,player->rad,RED);
            DrawCircle(ball->x,ball->y,ball->rad,RED);
            DrawText(TextFormat("SCORE: %i",(player->score)),10,10,22,BLACK);
            DrawText(TextFormat("HEALTH: %i",(player->hp)),10,30,22,BLACK);

            EndDrawing();
        } else if (game.state == "WIN") {
            BeginDrawing();
            ClearBackground(YELLOW);

            DrawText("YOU WIN!", worldX/2, worldY/2, 40, BLACK);

            EndDrawing();
        } else {
            BeginDrawing();
            ClearBackground(YELLOW);

            DrawText("YOU LOST!", worldX/2, worldY/2, 40, BLACK);

            EndDrawing();
        }
    }

    CloseWindow();
    
    return 0;
}