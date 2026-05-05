
#include "ui.h"

static UIButtons ui_buttons = { 0 };
static Vector2 mouse;

Button create_button(Rectangle rect, float rect_roundness, Color rect_color, const char* text, Color text_color, ButtonID id) {
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
        text, text_pos, text_width, false, id, 0
    };

    if (ui_buttons.counter < MAX_BUTTONS) {
        ui_buttons.buttons[ui_buttons.counter++] = button;
    }

    return button;
}

void update_button(Button* button) {
    button->pressed = false;

    if (button->press_timer > 0) {
        button->press_timer -= GetFrameTime();
        if (button->press_timer < 0) { button->press_timer = 0; }
    }

    if (CheckCollisionPointRec(mouse, button->rect)) {
        button->target = Fade(button->base_color, 0.8f);
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            button->pressed = true;
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

void ui_buttons_update(void) {
    mouse = GetMousePosition();

    for (int i = 0; i < ui_buttons.counter; i++) {
        update_button(&ui_buttons.buttons[i]);

        if (ui_buttons.buttons[i].pressed) {
            switch (ui_buttons.buttons[i].id) {
                case BUTTON_START:     TraceLog(LOG_ALL, "Button Pressed!"); break;
            }

            ui_buttons.buttons[i].pressed = false;
        }
    }
}

void ui_buttons_draw(void) {
    for (int i = 0; i < ui_buttons.counter; i++) {
        draw_button(&ui_buttons.buttons[i]);
    }
}