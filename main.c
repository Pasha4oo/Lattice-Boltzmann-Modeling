#include <raylib.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "consts.h"
#include "window.h"
#include "math_calculations.h"
#include "ui.h"

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

    bool* walls = malloc(Ny * Nx * sizeof(bool));
    /*  for (int i = 0; i < Ny * Nx; i++) {
          if (i < Nx || i > (Ny - 1) * Nx || i % Nx == 0 || i % Nx == Nx - 1) {
              walls[i] = true;
          }
          else {
              walls[i] = false;
          }
      }*/

    for (int i = 0; i < Ny * Nx; i++) {
        walls[i] = false;
    }

    set_cylinder_wall(walls, Nx / 2, Ny / 4, F);
    set_cylinder_wall(walls, Nx / 2, Ny / 3, F);
    set_cylinder_wall(walls, Nx / 2, Ny / 2, F);
    set_cylinder_wall(walls, Nx / 2 + 30, Ny, F);

    //for (int it = 0; it < Nt; it++) {
    //    //printf("%d\n", it);

    //    calculate(walls, F, F_next);

    //    //if (it % 100 == 0) {
    //    //    //save_velocity_to_csv(it, Nx, Ny, NL, F, cxs, cys);
    //    //    printf("Saved CSV for iteration %d\n", it);
    //    //}
    //}

    InitWindow(WINDOW_WIDTH, WINDOW_LENGTH, "LBM");

    SetTargetFPS(0);

    Image image = GenImageColor(Nx, Ny, BLACK);
    Texture2D texture = LoadTextureFromImage(image);

    uint32_t* pixels = (uint32_t*)malloc(Nx * Ny * sizeof(uint32_t));

    create_button((Rectangle) { Nx + 50, 50, 80, 50 }, 0.4f, ORANGE, "Text", BLUE, BUTTON_START);
    create_button((Rectangle) { Nx + 50, 190, 80, 50 }, 0.4f, RED, "Text", BLUE, BUTTON_START);

    double max_v = 1e-12;
    float timer = 0.0f;

    int pos_x = 40; int pos_y = 40;

    while (!WindowShouldClose())
    {
        calculate(walls, F, F_next);

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
        EndDrawing();

        //float mouseWheel = GetMouseWheelMove();
        //if (mouseWheel != 0)
        //{
        //    currentFps += (int)mouseWheel;
        //    if (currentFps < 0) currentFps = 0;
        //    SetTargetFPS(currentFps);
        //}

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Vector2 position = GetMousePosition();
            set_cylinder_wall(walls, position.x, position.y, F);
        }

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