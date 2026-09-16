#include "raylib.h"

int main()
{
    InitWindow(800, 450, "Raylib Test");

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Hello Raylib!", 300, 200, 30, BLACK);
        DrawRectangle(350, 250, 100, 100, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}