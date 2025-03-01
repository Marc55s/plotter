#include "raylib.h"
#include "raymath.h"


int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1800;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loopColorTint(GOLD, BLUE)
    // Define the camera to look into our 3d world
    Camera3D camera = {0};
    camera.position = (Vector3) {-10.0f, 5.0f, -20.0f}; // Camera position
    camera.target = (Vector3) {0.0f, 0.0f, 0.0f};      // Camera looking at point
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(0, 0);
        DrawGrid(100, 10);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context


    return 0;
}
