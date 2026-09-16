#include <raylib.h>
#include <iostream>
#include <string>

using namespace std;


struct Player {
    int x = 100;
    int y = 100;
    int speed = 10;
    int rad = 50;
};

struct ballObject {
    int x = 400;
    int y = 400;

    int vx = 10;
    int vy = 10;

    int rad = 60;

    void move() {x += vx; y += vy;}
};
class Game {
    public:
    void applyCollision (int wx, int wy, int* px, int* py, int rad) {
        cout << "POINTER INF py: " << *py << "\n";
        cout << "POINTER INF px: " << *px << "\n";
        if (wx - rad <= *px) {*px = wx - rad;}
        if (wy - rad <= *py) {*py = wy - rad;}
        if (0 + rad >= *px) {*px = 0 + rad;}
        if (0 + rad >= *py) {*py = 0 + rad;}
    }

    void collisionToBall (ballObject* ball,int wx, int wy) {
        if (wx - ball->rad <= ball->x) {ball->x = wx - ball->rad; ball->vx *= -1;}
        if (wy - ball->rad <= ball->y) {ball->y = wy - ball->rad;ball->vy *= -1;}
        if (0 + ball->rad >= ball->x) {ball->x = 0 + ball->rad;ball->vx *= -1;}
        if (0 + ball->rad >= ball->y) {ball->y = 0 + ball->rad;ball->vy *= -1;}
    }
};

int main () {
    InitWindow(1280,720,"GAME");
    SetTargetFPS(60);
    Game game;
    Player player;
    ballObject* ball = new ballObject;  // dangerous :| 
    int* px = &player.x;
    int* py = &player.y;
    int worldX = GetScreenWidth();
    int worldY = GetScreenHeight();

    cout << "POINTER INM py: " << *py << "\n";
    cout << "POINTER INM px: " << *px << "\n";

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_D)) {
            player.x += player.speed;
        }
        if (IsKeyDown(KEY_A)) {
            player.x -= player.speed;
        }
        if (IsKeyDown(KEY_W)) {
            player.y -= player.speed;
        }
        if (IsKeyDown(KEY_S)) {
            player.y += player.speed;
        }

        ball->move();


        game.applyCollision(worldX,worldY,px,py,player.rad);
        game.collisionToBall(ball,worldX,worldY);

        float dt = GetFrameTime();
    
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircle(player.x,player.y,player.rad,RED);
        DrawCircle(ball->x,ball->y,ball->rad,RED);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}