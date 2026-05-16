#include <raylib.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "consts.h"
#include "window.h"
#include "flow.h"
#include "ui.h"
#include "walls.h"
#include "files.h"

int main(void)
{
    srand(time(NULL));

    init_openmp();

    init_F();

    //push_liquid(F, 3);
    // push_liquid(F, 5);

    init_walls();

    //set_cylinder_wall(walls, Nx / 2, Ny / 4, F);
    //set_cylinder_wall(walls, Nx / 2, Ny / 3, F);
    //set_circle_wall((Vector2) { Nx / 2, Ny / 2 }, 60);
    //set_cylinder_wall(walls, Nx / 2 + 180, Ny, F);

    //for (int it = 0; it < Nt; it++) {
    //    //printf("%d\n", it);

    //    calculate(walls, F, F_next);

    //    //if (it % 100 == 0) {
    //    //    //save_velocity_to_csv(it, Nx, Ny, NL, F, cxs, cys);
    //    //    printf("Saved CSV for iteration %d\n", it);
    //    //}
    //}

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LBM");

    SetExitKey(KEY_NULL);

    SetTargetFPS(0);

    Image image = GenImageColor(Nx, Ny, BLACK);
    Texture2D texture = LoadTextureFromImage(image);

    uint32_t* pixels = (uint32_t*)malloc(Nx * Ny * sizeof(uint32_t));

    create_button((Rectangle) { 50 + WALLS_BASE_LENGTH, Ny + 20 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, ORANGE, "CIRCLE", BLUE, BUTTON_WALL_CIRCLE);
    create_button((Rectangle) { 50 + WALLS_BASE_LENGTH, Ny + 75 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, ORANGE, "FREE", BLUE, BUTTON_WALL_FREE);
    create_button((Rectangle) { 50 + WALLS_BASE_LENGTH, Ny + 130 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, ORANGE, "LINE", BLUE, BUTTON_WALL_LINE);
    create_button((Rectangle) { 140 + WALLS_BASE_LENGTH, Ny + 75 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, BLUE, "ERASE", BLUE, BUTTON_WALL_ERASER);
    create_button((Rectangle) { 140 + WALLS_BASE_LENGTH, Ny + 20 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, ORANGE, "POLY", BLUE, BUTTON_WALL_POLY);
    create_button((Rectangle) { 140 + WALLS_BASE_LENGTH, Ny + 130 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, RED, "CLEAR", BLUE, BUTTON_WALL_CLEAR);
    create_button((Rectangle) { 250 + WALLS_BASE_LENGTH, Ny + 20 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, BLUE, "BMP", BLUE, BUTTON_LOAD_BMP);
    create_button((Rectangle) { 250 + WALLS_BASE_LENGTH, Ny + 75 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, DARKGREEN, "LOAD", BLUE, BUTTON_LOAD_WALL);
    create_button((Rectangle) { 250 + WALLS_BASE_LENGTH, Ny + 130 + WALLS_BASE_HIGHT, 80, 50 }, 0.4f, DARKGREEN, "SAVE", BLUE, BUTTON_SAVE_WALL);
    
    create_button((Rectangle) { Nx + 130, 190, 80, 50 }, 0.4f, RED, "RESET", BLUE, BUTTON_RESTART_LIQUID);
   
    create_button((Rectangle) { 75 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT, 80, 50 }, 0.4f, ORANGE, "CLOG", BLUE, BUTTON_AREA_CLOGGED);
    create_button((Rectangle) { 75 + FILES_BASE_LENGTH, Ny + 75 + FILES_BASE_HIGHT, 80, 50 }, 0.4f, ORANGE, "CYCLIC", BLUE, BUTTON_AREA_CYCLIC);
    create_button((Rectangle) { 75 + FILES_BASE_LENGTH, Ny + 130 + FILES_BASE_HIGHT, 80, 50 }, 0.4f, ORANGE, "OUT", BLUE, BUTTON_AREA_OUTGOING);
    create_button((Rectangle) { 165 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT, 80, 50 }, 0.4f, DARKGREEN, "LOAD", BLUE, BUTTON_LOAD_ALL);
    create_button((Rectangle) { 165 + FILES_BASE_LENGTH, Ny + 75 + FILES_BASE_HIGHT, 80, 50 }, 0.4f, DARKGREEN, "SAVE", BLUE, BUTTON_SAVE_ALL);

    double max_v = 1e-12;
    float timer = 0.0f;

    int pos_x = 40; int pos_y = 40;

    while (!WindowShouldClose())
    {
        calculate(F, F_next);

        timer += GetFrameTime();

        if (timer >= 0.2f) {
            /*           for (int i = 0; i < Ny * Nx; i++) {
                           walls[i] = false;
                       }

                       set_cylinder_wall(walls, pos_x++, pos_y++, F);*/

            SetWindowTitle(TextFormat("FPS: %i", GetFPS()));
            update_max_v(&max_v, F);
            //push_liquid(F, 3);
            printf("local_max = %e\n", max_v);

            timer = 0.0f;
        }

        update_speed_pixels(F, pixels, max_v);
        UpdateTexture(texture, pixels);

        ui_buttons_update();

        BeginDrawing();
            ClearBackground((Color) { 172, 155, 135, 255 });
            DrawRectangleRounded((Rectangle) { 20, 10, Nx + 45, Ny + 30 }, 0.1f, 5, DARKBROWN);
            DrawRectangleRounded((Rectangle) { 35, 10, Nx + 30, Ny + 30 }, 0.1f, 5, BROWN);

            DrawTexture(texture, 50, 25, WHITE);

            DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, DARKGRAY);
            
            DrawRectangleRounded((Rectangle) { 20 + WALLS_BASE_LENGTH, Ny + 10 + WALLS_BASE_HIGHT, 425, 190 }, 0.1f, 5, DARKBROWN);
            DrawRectangleRounded((Rectangle) { 35 + WALLS_BASE_LENGTH, Ny + 10 + WALLS_BASE_HIGHT, 410, 180 }, 0.1f, 5, BROWN);
            DrawText("Walls", 390 + WALLS_BASE_LENGTH, Ny + 20 + WALLS_BASE_HIGHT, 15, RAYWHITE);

            DrawRectangleRounded((Rectangle) { 20 + FILES_BASE_LENGTH, Ny + 10 + FILES_BASE_HIGHT, 315, 190 }, 0.1f, 5, DARKBROWN);
            DrawRectangleRounded((Rectangle) { 35 + FILES_BASE_LENGTH, Ny + 10 + FILES_BASE_HIGHT, 300, 180 }, 0.1f, 5, BROWN);
            DrawText("Settings", 265 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT, 15, RAYWHITE);

            DrawCircleV((Vector2) { 55 + FILES_BASE_LENGTH, Ny + 75 + FILES_BASE_HIGHT + 25.0f }, 8.0f * 1.4f, Fade(GREEN, 0.3f));
            DrawCircleV((Vector2) { 55 + FILES_BASE_LENGTH, Ny + 75 + FILES_BASE_HIGHT + 25.0f }, 8.0f, GREEN);
            DrawCircleV((Vector2) { 53 + FILES_BASE_LENGTH, Ny + 75 + FILES_BASE_HIGHT + 23.0f }, 8.0f * 0.3f, Fade(WHITE, 0.7f));

            DrawCircleV((Vector2) { 55 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT + 25.0f }, 8.0f, DARKGRAY);
            DrawCircleV((Vector2) { 53 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT + 23.0f }, 8.0f * 0.2f, Fade(WHITE, 0.3f));

            ui_buttons_draw();
            walls_draw();
        EndDrawing();

        //float mouseWheel = GetMouseWheelMove();
        //if (mouseWheel != 0)
        //{
        //    currentFps += (int)mouseWheel;
        //    if (currentFps < 0) currentFps = 0;
        //    SetTargetFPS(currentFps);
        //}

        walls_update();
        //update_speed_pixels();

        //BeginDrawing();
        //    ClearBackground(RAYWHITE);
        //    DrawTexture(texture, 0, 0, WHITE);
        //    DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, DARKGRAY);

        //EndDrawing();
    }

    CloseWindow();

    free(F);
    free(F_next);
    free(walls);
    free(pixels);
    UnloadTexture(texture);

    return 0;
}