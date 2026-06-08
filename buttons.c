#include "buttons.h"

Button create_button(UIButtons* ui_buttons, Rectangle rect, float rect_roundness, Color rect_color, const char* text, Color text_color, ButtonCallback callback, void* callback_data) {
    int font_size = 20;
    int text_width = MeasureText(text, font_size);

    Vector2 text_pos = {
        rect.x + (rect.width / 2) - (text_width / 2.0f),
        rect.y + (rect.height / 2) - (font_size / 2.0f)
    };

    Button button = (Button)
    {
        rect, rect_roundness, rect_color,
        rect_color, rect_color,
        text, text_pos, text_width, 0,
        callback, callback_data
    };

    if (ui_buttons->counter < MAX_BUTTONS) {
        ui_buttons->buttons[ui_buttons->counter++] = button;
    }

    return button;
}

void update_button(Button* button) {
    Vector2 mouse = GetMousePosition();

    if (button->press_timer > 0) {
        button->press_timer -= GetFrameTime();
        if (button->press_timer < 0) { button->press_timer = 0; }
    }

    if (CheckCollisionPointRec(mouse, button->rect)) {
        button->target = Fade(button->base_color, 0.8f);
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (button->callback != NULL) {
                button->callback(button->callback_data);
            }
            button->press_timer = 1.0f;
            button->render_color = Fade(button->base_color, 0.4f);
        }
    }
    else {
        button->target = button->base_color;
    }

    float t;
    if (button->press_timer > 0) {
        t = 0.05f;
    }
    else {
        t = 0.15f;
    }

    button->render_color = ColorLerp(button->render_color, button->target, t);
}

void draw_button(Button* button) {
    DrawRectangleRounded(button->rect, 0.3f, 10, button->render_color);
    DrawTextEx(GetFontDefault(), button->text, button->text_pos, 20.0f, 2.0f, WHITE);
}

void ui_buttons_update(UIButtons* ui_buttons) {
    for (int i = 0; i < ui_buttons->counter; i++) {
        update_button(&ui_buttons->buttons[i]);
    }
}

void ui_buttons_draw(const UIButtons* ui_buttons) {
    for (int i = 0; i < ui_buttons->counter; i++) {
        draw_button(&ui_buttons->buttons[i]);
    }
}

void init_buttons(UIButtons* ui, Walls* ws, LBM* lbm, Pauser* pauser, Arrow* arrow, Settings* settings, LightSystem* light_system) {
    static LoadAllData load_all;
    static AreaTypeData area_type;
    load_all = (LoadAllData){ ws, lbm, arrow, settings, light_system };
    area_type = (AreaTypeData){ ws, settings, light_system };

    create_button(ui, (Rectangle) { 50 + WALLS_BASE_LENGTH, Ny + 20 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, ORANGE, "CIRCLE", BLUE, cb_wall_circle, ws);
    create_button(ui, (Rectangle) { 50 + WALLS_BASE_LENGTH, Ny + 75 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, ORANGE, "FREE", BLUE, cb_wall_free, ws);
    create_button(ui, (Rectangle) { 50 + WALLS_BASE_LENGTH, Ny + 130 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, ORANGE, "LINE", BLUE, cb_wall_line, ws);
    create_button(ui, (Rectangle) { 140 + WALLS_BASE_LENGTH, Ny + 75 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, BLUE, "ERASE", BLUE, cb_wall_eraser, ws);
    create_button(ui, (Rectangle) { 140 + WALLS_BASE_LENGTH, Ny + 20 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, ORANGE, "NONE", BLUE, cb_wall_none, ws);
    create_button(ui, (Rectangle) { 140 + WALLS_BASE_LENGTH, Ny + 130 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, RED, "CLEAR", BLUE, cb_clear_walls, ws);

    create_button(ui, (Rectangle) { 250 + WALLS_BASE_LENGTH, Ny + 20 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, BLUE, "BMP", BLUE, cb_load_bmp, ws);
    create_button(ui, (Rectangle) { 250 + WALLS_BASE_LENGTH, Ny + 75 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, DARKGREEN, "LOAD", BLUE, cb_load_wall, ws);
    create_button(ui, (Rectangle) { 250 + WALLS_BASE_LENGTH, Ny + 130 + WALLS_BASE_HIGHT, 80, 50 },
        0.4f, DARKGREEN, "SAVE", BLUE, cb_save_wall, ws);

    create_button(ui, (Rectangle) { Nx + 130, 35, 80, 80 },
        0.4f, BROWN, "", BLUE, cb_pauser, pauser);
    create_button(ui, (Rectangle) { Nx + 120, 180, 80, 50 },
        0.4f, RED, "RESET", BLUE, cb_restart_liquid, lbm);

    create_button(ui, (Rectangle) { 75 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT, 80, 50 },
        0.4f, ORANGE, "CLOG", BLUE, cb_area_clogged, &area_type);
    create_button(ui, (Rectangle) { 75 + FILES_BASE_LENGTH, Ny + 75 + FILES_BASE_HIGHT, 80, 50 },
        0.4f, ORANGE, "CYCLIC", BLUE, cb_area_cyclic, &area_type);
    create_button(ui, (Rectangle) { 75 + FILES_BASE_LENGTH, Ny + 130 + FILES_BASE_HIGHT, 80, 50 },
        0.4f, ORANGE, "OUT", BLUE, cb_area_outgoing, &area_type);

    create_button(ui, (Rectangle) { 165 + FILES_BASE_LENGTH, Ny + 20 + FILES_BASE_HIGHT, 80, 50 },
        0.4f, DARKGREEN, "LOAD", BLUE, cb_load_all, &load_all);
    create_button(ui, (Rectangle) { 165 + FILES_BASE_LENGTH, Ny + 75 + FILES_BASE_HIGHT, 80, 50 },
        0.4f, DARKGREEN, "SAVE", BLUE, cb_save_all, &load_all);

    create_button(ui, (Rectangle) { 165 + FILES_BASE_LENGTH, Ny + 130 + FILES_BASE_HIGHT, 80, 50 },
        0.4f, BLUE, "SCV", BLUE, cb_export_csv, lbm);
    create_button(ui, (Rectangle) { 10 + Nx + TAU_BASE_LENGTH, 230 + TAU_BASE_HIGHT, 50, 30 },
        0.4f, BLUE, "FIX", BLUE, cb_fixate, settings);
}