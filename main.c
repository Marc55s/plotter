#include "raylib.h"
#include "raymath.h"
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
#define GRID_WHITE (Color){120,120,120,255}


double SCALE = 50.0; // Scale factor (pixels per unit)

double function(double x) { return sin(x); }
double function2(double x) { return x; }
double quadratic(double x) { return x * x; }
double cool(double x) { return tan(x); }

void DrawAxes(Camera2D cam) {
    Vector2 worldMin = GetScreenToWorld2D((Vector2){0, 0}, cam);
    Vector2 worldMax = GetScreenToWorld2D((Vector2){WIDTH, HEIGHT}, cam);

    DrawLine(worldMin.x, 0, worldMax.x, 0, RED);  // X-axis
    DrawLine(0, worldMin.y, 0, worldMax.y, BLUE); // Y-axis

    DrawText("X", worldMax.x-20,10,20,RAYWHITE);
    DrawText("Y", -20,worldMin.y+10,20,RAYWHITE);
}

void DrawGrid2D(int slices, int spacing, Camera2D cam) {
    Vector2 worldMin = GetScreenToWorld2D((Vector2){0, 0}, cam);
    Vector2 worldMax = GetScreenToWorld2D((Vector2){WIDTH, HEIGHT}, cam);
    for (int i = -slices; i <= slices; i++) {
        DrawLine(spacing*i, worldMin.y, spacing*i, worldMax.y, GRID_WHITE);
        DrawLine(worldMin.x, spacing*i, worldMax.x, spacing*i, GRID_WHITE);  // X-axis
    }

}

void DrawGraph(double (*f)(double), Color color) {

    double delta_x = WIDTH / 2 / SCALE / 500;

    Vector2 points[10000];

    int index = 0;

    for (double x = -WIDTH / 2 / SCALE; x < WIDTH / 2 / SCALE; x += delta_x) {

        double y1 = f(x);
        double y2 = f(x + delta_x);

        if(fabs(y1-y2) < 100){
            Vector2 p1 = {x, -y1};
            Vector2 p2 = {(x + delta_x), -y2};
            p1 = Vector2Scale(p1, SCALE);
            p2 = Vector2Scale(p2, SCALE);
            points[index++] = p1;
            points[index++] = p2;
            DrawLineV(p1,p2,color);
        }else{
            TraceLog(LOG_INFO,"DIFF: (%lf)",fabs(y1-y2));
        }


    }
}

void MoveCam(Camera2D *cam) {
    double step = 3 * cam->zoom;
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
        DrawGrid2D(10000,50,cam);
        DrawAxes(cam);

        DrawGraph(function,YELLOW);
        DrawGraph(function2,DARKPURPLE);
        DrawGraph(quadratic,RED);
        DrawGraph(cool,RAYWHITE);

        DrawCircle(0, 0, 5, PURPLE); // Center dot (0,0)


        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
