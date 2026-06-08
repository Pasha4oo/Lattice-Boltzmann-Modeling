#include <raylib.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "consts.h"
#include "window.h"
#include "flow.h"
#include "buttons.h"
#include "buttons_callback.h"
#include "walls.h"
#include "files.h"
#include "lights.h"
#include "sliders.h"
#include "settings.h"
#include "pauser.h"
#include "arrow.h"

int main(void)
{
    LBM lbm = {
        .F = NULL,
        .F_next = NULL,
        .max_v = 1e-12
    };

    Walls walls = {
        .walls = NULL,
        .first_wall_line_pos = (Vector2){ -1, -1 },
        .walls_color = (Color){ 255, 255, 150, 255 },
        .wall_type = WALL_NONE,
        .brush_size = 14.0f
    };

    Pauser pauser = {
        .is_playing = true,
        .pos = (Vector2) { Nx + 147, 50},
        .size = (Vector2) { 45.0f, 45.0f }
    };

    Settings settings = {
        .area_type = AREA_OUTGOING,
        .flow_speed = 0.07f,
        .tau = .57, //0.53
        .fixate = false
    };

    UIButtons ui_buttons = { 0 };
    UISliders ui_sliders = { 0 };

    srand(time(NULL));

    init_openmp();

    init_lbm(&lbm);

    //push_liquid(F, 3);

    init_walls(&walls.walls);

    set_circle_wall((Vector2) { Nx / 2, Ny / 2 }, &walls);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LBM");

    SetExitKey(KEY_NULL);

    SetTargetFPS(0);

    Image image = GenImageColor(Nx, Ny, BLACK);
    Texture2D texture = LoadTextureFromImage(image);

    uint32_t* pixels = (uint32_t*)malloc(Nx * Ny * sizeof(uint32_t));

    Arrow arrow = create_arrow((Vector2) { 60 + Nx + ARROW_BASE_LENGTH, 150 + Ny + ARROW_BASE_HIGHT }, 100.0f, RAYWHITE);


    LightSystem light_system = (LightSystem){ NULL, -1, 0 };
    create_light(&light_system, (Vector2) { 55 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT + 25.0f });
    create_light(&light_system, (Vector2) { 55 + FILES_BASE_LENGTH, Ny + 75 + FILES_BASE_HIGHT + 25.0f });
    create_light(&light_system, (Vector2) { 55 + FILES_BASE_LENGTH, Ny + 130 + FILES_BASE_HIGHT + 25.0f });
    light_system.enabled_index = 2;

    init_buttons(&ui_buttons, &walls, &lbm, &pauser, &arrow, &settings, &light_system);

    LightSystem light_system2 = (LightSystem){ NULL, -1, 0 };
    create_light(&light_system2, (Vector2) { 130 + Nx + TAU_BASE_LENGTH, 80 + TAU_BASE_HIGHT + 25.0f });

    init_sliders(&ui_sliders, &walls, &settings);

    float timer = 0.0f;

    int pos_x = 40; int pos_y = 40;
    bool explode_flicker = false;

    while (!WindowShouldClose())
    {
        if (pauser.is_playing) { calculate(&lbm, &walls, &settings, &arrow); }

        timer += GetFrameTime();

        if (timer >= 0.2f) {
            SetWindowTitle(TextFormat("FPS: %i", GetFPS()));

            if (pauser.is_playing && !settings.fixate) { update_max_v(&lbm); }
            if (lbm.max_v > 200 && !explode_flicker) {
                light_system2.enabled_index = 0; 
                explode_flicker = true;
            }
            else { 
                light_system2.enabled_index = -1; 
                explode_flicker = false;
            }

            printf("local_max = %e\n", lbm.max_v);
            printf("tau = %e\n", settings.tau);

            timer = 0.0f;
        }

        update_speed_pixels(&lbm, &walls, pixels);
        UpdateTexture(texture, pixels);

        update_arrow(&arrow);
        ui_buttons_update(&ui_buttons);
        ui_sliders_update(&ui_sliders, &settings, &walls);

        BeginDrawing();
            ClearBackground((Color) { 172, 155, 135, 255 });
            DrawRectangleRounded((Rectangle) { 20, 10, Nx + 45, Ny + 30 }, 0.1f, 5, DARKBROWN);
            DrawRectangleRounded((Rectangle) { 35, 10, Nx + 30, Ny + 30 }, 0.1f, 5, BROWN);

            DrawTexture(texture, 50, 25, WHITE);

            DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, DARKGRAY);
            
            DrawRectangleRounded((Rectangle) { 20 + WALLS_BASE_LENGTH, Ny + 10 + WALLS_BASE_HIGHT, 425, 190 }, 0.1f, 5, DARKBROWN);
            DrawRectangleRounded((Rectangle) { 35 + WALLS_BASE_LENGTH, Ny + 10 + WALLS_BASE_HIGHT, 410, 180 }, 0.1f, 5, BROWN);
            DrawText("Walls", 390 + WALLS_BASE_LENGTH, Ny + 20 + WALLS_BASE_HIGHT, 15, RAYWHITE);
            DrawText("Brush \nSize:", 390 + WALLS_BASE_LENGTH, Ny + 70 + WALLS_BASE_HIGHT, 15, RAYWHITE);
            DrawText(TextFormat("%d p", (int)(walls.brush_size)), 390 + WALLS_BASE_LENGTH, Ny + 110 + WALLS_BASE_HIGHT, 19, RAYWHITE);

            DrawRectangleRounded((Rectangle) { 20 + FILES_BASE_LENGTH, Ny + 10 + FILES_BASE_HIGHT, 355, 190 }, 0.1f, 5, DARKBROWN);
            DrawRectangleRounded((Rectangle) { 35 + FILES_BASE_LENGTH, Ny + 10 + FILES_BASE_HIGHT, 340, 180 }, 0.1f, 5, BROWN);
            DrawText("Settings", 265 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT, 15, RAYWHITE);
            DrawText("Flow \nSpeed:", 305 + FILES_BASE_LENGTH, Ny + 70 + FILES_BASE_HIGHT, 15, RAYWHITE);
            if (settings.area_type == AREA_OUTGOING) {
                DrawText(TextFormat("%.2f", (settings.flow_speed)), 305 + FILES_BASE_LENGTH, Ny + 110 + FILES_BASE_HIGHT, 19, RAYWHITE);
            }
            else {
                DrawText("0", 305 + FILES_BASE_LENGTH, Ny + 110 + FILES_BASE_HIGHT, 19, RED);
            }

            ui_lights_draw(&light_system);

            DrawRectangleRounded((Rectangle) { 20 + Nx + ARROW_BASE_LENGTH, Ny + ARROW_BASE_HIGHT + 20, 195, 240 }, 0.1f, 5, DARKBROWN);
            DrawRectangleRounded((Rectangle) { 35 + Nx + ARROW_BASE_LENGTH, Ny + ARROW_BASE_HIGHT + 20, 180, 230 }, 0.1f, 5, BROWN);
            DrawText("Angle", Nx + 150 + ARROW_BASE_LENGTH, Ny + 30 + ARROW_BASE_HIGHT, 15, RAYWHITE);
            draw_arrow(&arrow);
            DrawText(TextFormat("%d", (int)(arrow.angle* (180.0f / M_PI) + (arrow.angle >= 0 ? 0.5f : -0.5f))), Nx + 100 + ARROW_BASE_LENGTH, Ny + 30 + ARROW_BASE_HIGHT, 19, RAYWHITE);

            DrawRectangleRounded((Rectangle) { 20 + Nx + TAU_BASE_LENGTH - 40, 50 + TAU_BASE_HIGHT, 195, 240 }, 0.1f, 5, DARKBROWN);
            DrawRectangleRounded((Rectangle) { 35 + Nx + TAU_BASE_LENGTH - 40, 50 + TAU_BASE_HIGHT, 180, 230 }, 0.1f, 5, BROWN);
            DrawText("Flow", 120 + Nx + TAU_BASE_LENGTH, 60 + TAU_BASE_HIGHT, 15, RAYWHITE);
            DrawText("TAU:", 85 + Nx + TAU_BASE_LENGTH, 170 + TAU_BASE_HIGHT, 19, RAYWHITE);
            DrawText(TextFormat("%.2f", (settings.tau)), 90 + Nx + TAU_BASE_LENGTH, 200 + TAU_BASE_HIGHT, 19, RAYWHITE);

            DrawText("Max", 15 + Nx + TAU_BASE_LENGTH, 160 + TAU_BASE_HIGHT, 19, RAYWHITE);
            DrawText("Vel:", 15 + Nx + TAU_BASE_LENGTH, 180 + TAU_BASE_HIGHT, 19, RAYWHITE);
            if (settings.fixate) {
                DrawText(TextFormat("%.3f", (lbm.max_v)), 15 + Nx + TAU_BASE_LENGTH, 205 + TAU_BASE_HIGHT, 19, RED);
            }
            else {
                DrawText(TextFormat("%.3f", (lbm.max_v)), 15 + Nx + TAU_BASE_LENGTH, 205 + TAU_BASE_HIGHT, 19, RAYWHITE);
            }

            ui_lights_draw(&light_system2);

            DrawRectangleRounded((Rectangle) { Nx + 120, 35, 90, 90 }, 0.4f, 5, DARKBROWN);
            ui_buttons_draw(&ui_buttons);
            draw_pauser(&pauser);
            ui_sliders_draw(&ui_sliders);
            walls_draw(&walls);
        EndDrawing();

        walls_update(&walls);
    }

    CloseWindow();

    free(lbm.F);
    free(lbm.F_next);
    free(walls.walls);
    free(pixels);
    free(light_system.lights);
    free(light_system2.lights);
    UnloadTexture(texture);

    return 0;
}