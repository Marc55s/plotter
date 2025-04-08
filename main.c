#include "raylib.h"
#include "raymath.h"
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
#define GRID_WHITE (Color){120, 120, 120, 255}

double sinus(double x) { return sin(x); }
double quad(double x) { return x * x; }

void DrawGraph(double (*func)(double), Camera2D cam) {
    Vector2 worldMin = GetScreenToWorld2D((Vector2){0, 0}, cam);
    Vector2 worldMax = GetScreenToWorld2D((Vector2){WIDTH, HEIGHT}, cam);

    double DELTA_X =
        (fabs(worldMin.x - worldMax.x)) /
        5000; // guarantees same amount of steps, for any width of x-axis

    for (double i = worldMin.x; i < worldMax.x; i += DELTA_X) {
        double y = func(i);
        double y2 = func(i + DELTA_X);

        if (fabs(cos(i)) > 0.001 && fabs(y - y2) < 100) {
            Vector2 start = (Vector2){i, -y};
            Vector2 end = (Vector2){i + DELTA_X, -y2};
            DrawLineV(start, end, RAYWHITE);
        }
    }
}

void DrawAxes(Camera2D camera) {
    Vector2 worldMin = GetScreenToWorld2D((Vector2){0, 0}, camera);
    Vector2 worldMax = GetScreenToWorld2D((Vector2){WIDTH, HEIGHT}, camera);

    // TraceLog(LOG_INFO,"worldMin: (%f, %f), worldMax: (%f, %f)\n", worldMin.x,
    // worldMin.y, worldMax.x, worldMax.y);
    DrawLine(worldMin.x - 1, 0, worldMax.x + 1, 0, GOLD); // X-Axis
    DrawLine(0, worldMin.y - 1, 0, worldMax.y + 1, GOLD); // Y-Axis
}

void DrawGrid2D(Camera2D camera) {
    Vector2 worldMin = GetScreenToWorld2D((Vector2){0, 0}, camera);
    Vector2 worldMax = GetScreenToWorld2D((Vector2){WIDTH, HEIGHT}, camera);

    // Adjust grid spacing based on the zoom level
    double gridSpacing = 1.0 / camera.zoom;  // Scale grid based on zoom
    
    // Set limits to avoid too fine or too coarse grid lines
    if (gridSpacing < 0.1) gridSpacing = 0.1;   // Minimum grid spacing
    if (gridSpacing > 5.0) gridSpacing = 5.0;   // Maximum grid spacing

    // Y Grid lines (vertical lines)
    for (double i = floor(worldMin.x / gridSpacing) * gridSpacing; i < worldMax.x; i += gridSpacing) {
        DrawLineV((Vector2){i, worldMin.y}, (Vector2){i, worldMax.y}, GRAY);
    }

    // X Grid lines (horizontal lines)
    for (double i = floor(worldMin.y / gridSpacing) * gridSpacing; i < worldMax.y; i += gridSpacing) {
        DrawLineV((Vector2){worldMin.x, i}, (Vector2){worldMax.x, i}, GRAY);
    }
}

void UpdateCameraControl(Camera2D *camera) {
    Vector2 worldBeforeZoom =
        GetScreenToWorld2D((Vector2){WIDTH / 2.0f, HEIGHT / 2.0f}, *camera);

    TraceLog(LOG_INFO, "zoom at > %f", camera->zoom);
    if (IsKeyDown(KEY_E)) {
        camera->zoom *= 1.1;
    }
    if (IsKeyDown(KEY_Q)) {
        camera->zoom /= 1.1;
    }

    Vector2 worldAfterZoom =
        GetScreenToWorld2D((Vector2){WIDTH / 2.0f, HEIGHT / 2.0f}, *camera);

    camera->target.x -= (worldAfterZoom.x - worldBeforeZoom.x);
    camera->target.y -= (worldAfterZoom.y - worldBeforeZoom.y);

    float step = 8;

    if (IsKeyDown(KEY_W)) {
        camera->offset.y += step;
    }
    if (IsKeyDown(KEY_A)) {
        camera->offset.x += step;
    }
    if (IsKeyDown(KEY_S)) {
        camera->offset.y -= step;
    }
    if (IsKeyDown(KEY_D)) {
        camera->offset.x -= step;
    }
}


int main() {
    InitWindow(WIDTH, HEIGHT, "Function Plotter");
    SetTargetFPS(60);
    SetGesturesEnabled(GESTURE_PINCH_IN | GESTURE_PINCH_OUT);
    Vector2 offset = {WIDTH / 2, HEIGHT / 2};
    Vector2 target = {0};
    Camera2D camera = (Camera2D){offset, target, 0, 10};

    while (!WindowShouldClose()) {
        UpdateCameraControl(&camera);

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(BLACK);

        DrawGrid2D(camera);
        DrawAxes(camera);
        DrawGraph(sinus, camera);
        DrawGraph(cos, camera);
        DrawGraph(quad, camera);
        /*DrawGraph(tan, camera);*/
        /*DrawCircle(0, 0, 10, RED);*/

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
