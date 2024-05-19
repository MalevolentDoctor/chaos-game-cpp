#include "raylib/raylib.h"
#include "ChaosGame.h"

int WinMain() {
    //SetTraceLogLevel(4);
    InitWindow(1920, 1080, "Chaos Game");

    // Declare objects
    ChaosGame cg(5);


    while (!WindowShouldClose()) {
        cg.input();
        cg.update();

        BeginDrawing();
        ClearBackground(BLACK);

        cg.draw();
        DrawFPS(5, 5);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}