#include "raylib.h"
#include "raymath.h"
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
float SCALE = 50.0; // Scale factor (pixels per unit)

float function(float x) { return sin(x); }

float function2(float x) { return x; }

void DrawAxes(Camera2D cam) {
    Vector2 worldMin = GetScreenToWorld2D((Vector2){0, 0}, cam);
    Vector2 worldMax = GetScreenToWorld2D((Vector2){WIDTH, HEIGHT}, cam);

    DrawLine(worldMin.x, 0, worldMax.x, 0, RED);  // X-axis
    DrawLine(0, worldMin.y, 0, worldMax.y, BLUE); // Y-axis

    DrawText("X", worldMax.x-20,10,20,RAYWHITE);
    DrawText("Y", -20,worldMin.y+10,20,RAYWHITE);
}

void DrawGrid2D(int slices, int spacing, Camera2D *cam) {
}

void DrawGraph(float (*f)(float)) {

    float delta_x = WIDTH / 2 / SCALE / 100;

    Vector2 points[10000];

    int index = 0;

    for (float x = -WIDTH / 2 / SCALE; x < WIDTH / 2 / SCALE; x += delta_x) {

        float y1 = f(x);
        float y2 = f(x + delta_x);

        Vector2 p1 = {x, -y1};
        Vector2 p2 = {(x + delta_x), -y2};
        p1 = Vector2Scale(p1, SCALE);
        p2 = Vector2Scale(p2, SCALE);

        /*p1 = Vector2Subtract(p1, (Vector2){WIDTH/2,0});*/
        /*p2 = Vector2Subtract(p2, (Vector2){WIDTH/2,0});*/

        points[index] = p1;
        points[index++] = p2;

        /*DrawLineV(p1, p2, RAYWHITE);*/
    }
    DrawSplineCatmullRom(points, index, 1, RAYWHITE);
}

void MoveCam(Camera2D *cam) {
    float step = 3 * cam->zoom;
    if (IsKeyDown(KEY_W)) {
        cam->offset.y += step;
    }
    if (IsKeyDown(KEY_A)) {
        cam->offset.x += step;
    }
    if (IsKeyDown(KEY_S)) {
        cam->offset.y -= step;
    }
    if (IsKeyDown(KEY_D)) {
        cam->offset.x -= step;
    }
    if (IsKeyDown(KEY_Q)) {
        if (cam->zoom > 1)
            cam->zoom = sqrt(cam->zoom);
    }
    if (IsKeyDown(KEY_E)) {
        cam->zoom += 0.1;
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Function Plotter");
    SetTargetFPS(60);
    Vector2 target = {0, 0};
    Vector2 offset = {WIDTH / 2, HEIGHT / 2};
    Camera2D cam = (Camera2D){offset, target, 0, 1};

    while (!WindowShouldClose()) {
        BeginDrawing();
        BeginMode2D(cam);

        ClearBackground(DARKGRAY);

        MoveCam(&cam);
        DrawAxes(cam);

        DrawGraph(function);
        DrawGraph(function2);

        DrawCircle(0, 0, 5, PURPLE); // Center dot (0,0)

        // Window Frame
        /*
    DrawLine(0,0,WIDTH,0,GOLD);
    DrawLine(WIDTH,0, WIDTH, HEIGHT,GOLD);
    DrawLine(WIDTH, HEIGHT, 0, HEIGHT,GOLD);
    DrawLine(0, HEIGHT, 0,0,GOLD);
    */

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
