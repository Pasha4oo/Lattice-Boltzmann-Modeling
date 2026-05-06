#include <raylib.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "consts.h"
#include "window.h"
#include "math_calculations.h"
#include "ui.h"
#include "walls.h"

int main(void)
{
    srand(time(NULL));

    init_openmp();

    double* F = malloc(Ny * Nx * NL * sizeof(double));
    double* F_next = malloc(Ny * Nx * NL * sizeof(double));

    for (int i = 0; i < Ny * Nx * NL; i++) {
        F[i] = 1.0 + 0.01 * randn();
    }

    //push_liquid(F, 3);
    // push_liquid(F, 5);

    init_walls();

    //set_cylinder_wall(walls, Nx / 2, Ny / 4, F);
    //set_cylinder_wall(walls, Nx / 2, Ny / 3, F);
    set_circle_wall((Vector2) { Nx / 2, Ny / 2 }, 60);
    //set_cylinder_wall(walls, Nx / 2 + 180, Ny, F);

    //for (int it = 0; it < Nt; it++) {
    //    //printf("%d\n", it);

    //    calculate(walls, F, F_next);

    //    //if (it % 100 == 0) {
    //    //    //save_velocity_to_csv(it, Nx, Ny, NL, F, cxs, cys);
    //    //    printf("Saved CSV for iteration %d\n", it);
    //    //}
    //}

    InitWindow(WINDOW_WIDTH, WINDOW_LENGTH, "LBM");

    SetExitKey(KEY_NULL);

    SetTargetFPS(0);

    Image image = GenImageColor(Nx, Ny, BLACK);
    Texture2D texture = LoadTextureFromImage(image);

    uint32_t* pixels = (uint32_t*)malloc(Nx * Ny * sizeof(uint32_t));

    create_button((Rectangle) { Nx + 50, 50, 80, 50 }, 0.4f, ORANGE, "CIRCLE", BLUE, BUTTON_WALL_CIRCLE);
    create_button((Rectangle) { Nx + 50, 190, 80, 50 }, 0.4f, RED, "RESET", BLUE, BUTTON_START);
    create_button((Rectangle) { Nx + 50, 290, 80, 50 }, 0.4f, RED, "FREE", BLUE, BUTTON_WALL_FREE);
    create_button((Rectangle) { Nx + 50, 390, 80, 50 }, 0.4f, RED, "LINE", BLUE, BUTTON_WALL_LINE);
    create_button((Rectangle) { Nx + 50, 450, 80, 50 }, 0.4f, RED, "ERASE", BLUE, BUTTON_WALL_ERASER);
    create_button((Rectangle) { Nx + 50, 550, 80, 50 }, 0.4f, RED, "POLY", BLUE, BUTTON_WALL_POLY);

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
            ClearBackground(RAYWHITE);
            DrawTexture(texture, 0, 0, WHITE);
            DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, DARKGRAY);
            
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