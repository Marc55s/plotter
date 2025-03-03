#include "raylib.h"
#include "raymath.h"
#include <math.h>

#define WIDTH 1200
#define HEIGHT 800
#define SCALE 50.0 // Scale factor (pixels per unit)

float function(float x) { 
    return sin(x);
}

float function2(float x) { 
    return cos(x);
}


void DrawAxes() {
    DrawLine(0, -HEIGHT/2, 0, HEIGHT/2, BLUE);  // Y-axis
    DrawLine(-WIDTH/2, 0, WIDTH/2, 0, BLUE); // X-axis
}

void DrawGrid2D(int slices, int spacing ) {
    
    for (int i = -slices; i < slices; i++) {
        DrawLine((i+1)*spacing,-HEIGHT,(i+1)*spacing,HEIGHT,BLUE);
        char str[10];
        DrawText(TextFormat("%.2f",(i+1)*spacing/SCALE),(i+1)*spacing,0,20,RAYWHITE);
    }
    for (int j = -slices; j < slices; j++) {
        DrawLine(-WIDTH,(j+1)*spacing,WIDTH,(j+1)*spacing,BLUE);
        DrawText(TextFormat("%.2f",(j+1)*spacing/SCALE),0,(j+1)*spacing,20,RAYWHITE);
    }   
}

void DrawGraph(float (*f)(float)) {

    float delta_x = WIDTH/2/SCALE / 100;

    Vector2 points[10000];

    int index = 0;


    for (float x = -WIDTH/2/SCALE; x <  WIDTH/2/SCALE;x += delta_x) {

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
    DrawSplineCatmullRom(points,index,1,RAYWHITE);
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
        DrawAxes();
        DrawGrid2D(100,100);

        DrawGraph(function);
        DrawGraph(function2);

        DrawCircle(0,0,5,PURPLE); // Center dot (0,0)

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
