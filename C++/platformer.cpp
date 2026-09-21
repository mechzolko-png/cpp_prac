#include <raylib.h>

#include <iostream>
#include <string>
#include <cmath>

struct Platform {
    public:
    float x;
    float y;
    float width;
    float height;

    Platform (float x,float y,float width,float height):
        x(x), y(y), width(width), height(height) {}

    void drawMe () {
        DrawRectangle(x,y,width,height,BLACK);
    }
};

class Player {
    public:
    float x;
    float y;
    float vx;
    float vy;
    float speed;
    float acc;
    float gravity;
    float friction;
    int rad;
    bool grounded;

    Player (float x,float y,float speed):
        x(x), y(y), speed(speed) {

        vx = 0;
        vy = 0;
        acc = 1;
        gravity = 2;
        friction = 0.8;
        rad = 20;
        grounded = false;
    }

    void move () {
        vx *= friction;
        vy += gravity;

        if (IsKeyDown(KEY_A)) {
            vx -= acc;
        }

        if (IsKeyDown(KEY_D)) {
            vx += acc;
        }

        if (IsKeyPressed(KEY_SPACE) && grounded) {
            vy = -50;
            grounded = false;
        }

        if (vx > speed) {
            vx = speed;
        }

        if (vx < -speed) {
            vx = -speed;
        }

        x += vx;
        y += vy;
    }

    void collision (Platform* platforms, int platformCount, int worldWidth, int worldHeight) {
        grounded = false;

        if (x - rad <= 0) {
            x = rad;
            vx = 0;
        }

        if (x + rad >= worldWidth) {
            x = worldWidth - rad;
            vx = 0;
        }

        if (y - rad <= 0) {
            y = rad;
            vy = 0;
        }

        if (y + rad >= worldHeight) {
            y = worldHeight - rad;
            vy = 0;
            grounded = true;
        }

        for (int i = 0; i < platformCount; i++) {
            Platform& platform = platforms[i];

            if (
                vy >= 0 &&
                x + rad > platform.x &&
                x - rad < platform.x + platform.width &&
                y + rad >= platform.y &&
                y - rad < platform.y
            ) {
                y = platform.y - rad;
                vy = 0;
                grounded = true;
            }

            if (
                vy < 0 &&
                x + rad > platform.x &&
                x - rad < platform.x + platform.width &&
                y - rad <= platform.y + platform.height &&
                y + rad > platform.y + platform.height
            ) {
                y = platform.y + platform.height + rad;
                vy = 0;
            }
        }
    }
};

int main () {
    InitWindow(1280,720,"Platformer");
    SetTargetFPS(60);

    Player* player = new Player(200,300,10);

    Platform platforms[] = {
        Platform(0,650,1280,70),
        Platform(250,520,250,20),
        Platform(650,430,250,20),
        Platform(1000,330,250,20),
        Platform(1350,500,300,20),
        Platform(1750,380,250,20),
        Platform(2150,550,350,20)
    };

    int platformCount = sizeof(platforms) / sizeof(platforms[0]);

    Camera2D camera = { 0 };
    camera.offset = {
        GetScreenWidth() / 2.0f,
        GetScreenHeight() / 2.0f
    };
    camera.target = {
        player->x,
        player->y
    };
    camera.zoom = 1.0f;

    while (!WindowShouldClose()) {

        player->move();

        player->collision(
            platforms,
            platformCount,
            2500,
            720
        );

        camera.target.x = player->x;
        camera.target.y = player->y;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        for (int i = 0; i < platformCount; i++) {
            platforms[i].drawMe();
        }

        DrawCircle(
            player->x,
            player->y,
            player->rad,
            RED
        );

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();

    delete player;

    return 0;
}