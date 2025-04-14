#include "raylib.h"
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
#define GRID_WHITE (Color){120, 120, 120, 255}

double sinus(double x) { return sin(x); }
double quad(double x) { return 3*x * x *x - 2 *x*x - 8 *x; }


void DrawGraph(double (*func)(double), Camera2D cam, Color color) {
    Vector2 worldMin = GetScreenToWorld2D((Vector2){0, 0}, cam);
    Vector2 worldMax = GetScreenToWorld2D((Vector2){WIDTH, HEIGHT}, cam);

    double DELTA_X = (fabs(worldMin.x - worldMax.x)) / 5000; // guarantees same amount of steps, for any width of x-axis

    for (double i = worldMin.x; i < worldMax.x; i += DELTA_X) {
        double y = func(i);
        double y2 = func(i + DELTA_X);

        if(isfinite(y) && isfinite(y2)) {
            Vector2 start = (Vector2){i, -y};
            Vector2 end = (Vector2){i + DELTA_X, -y2};
            DrawLineV(start, end, color);
        }else {
            TraceLog(LOG_DEBUG, "Non Finite Function");
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

void DrawSmartGrid2D(Camera2D camera, float baseGridSpacing, int majorLineEveryN, Color minorColor, Color majorColor) {
    Vector2 screenMin = {0, 0};
    Vector2 screenMax = {WIDTH, HEIGHT};

    Vector2 worldMin = GetScreenToWorld2D(screenMin, camera);
    Vector2 worldMax = GetScreenToWorld2D(screenMax, camera);

    // Adaptive spacing based on zoom
    float zoom = camera.zoom;
    float rawSpacing = baseGridSpacing / zoom;

    // Snap spacing to "nice" round numbers: 1, 2, 5, 10, etc.
    float magnitude = powf(10.0f, floorf(log10f(rawSpacing)));
    float normalized = rawSpacing / magnitude;

    float niceSpacing;
    if (normalized < 1.5f)
        niceSpacing = 1.0f;
    else if (normalized < 3.0f)
        niceSpacing = 2.0f;
    else if (normalized < 7.0f)
        niceSpacing = 5.0f;
    else
        niceSpacing = 10.0f;

    float spacing = niceSpacing * magnitude;

    // prevent float rounding jiggle
    float startX = floorf(worldMin.x / spacing) * spacing;
    float endX = ceilf(worldMax.x / spacing) * spacing;
    float startY = floorf(worldMin.y / spacing) * spacing;
    float endY = ceilf(worldMax.y / spacing) * spacing;

    // Vertical lines
    //
    for (float x = startX; x <= endX; x += spacing) {
        int index = (int)roundf(x / spacing);
        bool isMajor = (index % majorLineEveryN == 0);
        DrawLineV((Vector2){x, worldMin.y}, (Vector2){x, worldMax.y}, isMajor ? majorColor : minorColor);

        // Label
        if (isMajor) {
            const char *label_text = TextFormat("%.3f", x);
            Vector2 label_pos = {x + spacing , 0}; // offset a bit
            DrawTextPro(GetFontDefault(), label_text, label_pos, (Vector2){0, 0}, 0.0f, spacing * 0.4, spacing * 0.1f, WHITE);
        }
    }

    // Horizontal lines
    for (float y = startY; y <= endY; y += spacing) {
        int index = (int)roundf(y / spacing);
        bool isMajor = (index % majorLineEveryN == 0);
        DrawLineV((Vector2){worldMin.x, y}, (Vector2){worldMax.x, y}, isMajor ? majorColor : minorColor);

        // Label
        if (isMajor) {
            const char *label_text = TextFormat("%.3f", y);
            Vector2 label_pos = {0, y + spacing * 0.5f};

            DrawTextPro(GetFontDefault(), label_text, label_pos, (Vector2){0, 0}, 0.0f, spacing * 0.4, spacing * 0.1f, WHITE);
        }
    }
}

void UpdateCameraControl(Camera2D *camera) {
    Vector2 worldBeforeZoom =
        GetScreenToWorld2D((Vector2){WIDTH / 2.0f, HEIGHT / 2.0f}, *camera);

    // TraceLog(LOG_INFO, "zoom at > %f", camera->zoom);
    if (IsKeyDown(KEY_E)) {
        if(camera->zoom < 1000000)
            camera->zoom *= 1.1;
    }
    if (IsKeyDown(KEY_Q)) {
        if(camera->zoom > 0.000001)
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

        DrawSmartGrid2D(camera, 32.0, 5, DARKGRAY, LIGHTGRAY);
        DrawAxes(camera);
        DrawGraph(sinus, camera,LIME);
        DrawGraph(cos, camera,GOLD);
        DrawGraph(quad, camera,YELLOW);
        DrawGraph(exp2, camera,YELLOW);

        /*DrawGraph(tan, camera);*/
        /*DrawCircle(0, 0, 10, RED);*/

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
