#include "raylib.h"
#include "raymath.h"
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
#define SCALE 50.0 // Scale factor (pixels per unit)

float function(float x) { 
    return sin(x) * 3;
}

void DrawAxes() {
    DrawLine(0, -HEIGHT/2, 0, HEIGHT/2, BLUE);  // Y-axis
    DrawLine(-WIDTH/2, 0, WIDTH/2, 0, BLUE); // X-axis
}

void DrawGraph() {
    DrawAxes();
    float delta_x = 0.01;

    Vector2 points[4801];

    int index = 0;


    for (float x =  -WIDTH/2/SCALE; x <  WIDTH/2/SCALE;x += delta_x) {

        float y1 = function(x);
        float y2 = function(x + delta_x);

        Vector2 p1 = {x, -y1};
        Vector2 p2 = {(x + delta_x), -y2};
        p1 = Vector2Scale(p1, SCALE);
        p2 = Vector2Scale(p2, SCALE);

        /*p1 = Vector2Subtract(p1, (Vector2){WIDTH/2,0});*/
        /*p2 = Vector2Subtract(p2, (Vector2){WIDTH/2,0});*/

        points[index] = p1;
        points[index+1] = p2;
        
        index+=2;

        /*DrawLineV(p1, p2, RAYWHITE);*/
    }
    TraceLog(LOG_INFO,"%d\n",index);
    DrawSplineCatmullRom(points,4801,5,RAYWHITE);
}

void MoveCam(Camera2D *cam) {
    float step = 3 * cam->zoom;
    if (IsKeyDown(KEY_W)){
        cam->offset.y += step;
    }
    if (IsKeyDown(KEY_A)){
        cam->offset.x += step;
    }
    if (IsKeyDown(KEY_S)){
        cam->offset.y -= step;
    }
    if (IsKeyDown(KEY_D)){
        cam->offset.x -= step;
    }
    if (IsKeyDown(KEY_Q)){
        if(cam->zoom > 1)
            cam->zoom -= 1;
    }
    if (IsKeyDown(KEY_E)){
        cam->zoom += 1;
    }
}
int main() {
    InitWindow(WIDTH, HEIGHT, "Function Plotter");
    SetTargetFPS(60);
    Vector2 target = {0, 0};
    Vector2 offset = {WIDTH/2,HEIGHT/2};
    Camera2D cam = (Camera2D){offset, target, 0, 1};

    while (!WindowShouldClose()) {
        BeginDrawing();
        BeginMode2D(cam);

        ClearBackground(DARKGRAY);

        MoveCam(&cam);
        DrawGraph();

        DrawCircle(0,0,10,PURPLE); // Center dot (0,0)

        // Window Frame
        DrawLine(0,0,WIDTH,0,GOLD);
        DrawLine(WIDTH,0, WIDTH, HEIGHT,GOLD);
        DrawLine(WIDTH, HEIGHT, 0, HEIGHT,GOLD);
        DrawLine(0, HEIGHT, 0,0,GOLD);

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
