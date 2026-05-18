#include "arrow.h"

Arrow arrow;
static Vector2 mouse;

int ARROW_BASE_LENGTH = 60;
int ARROW_BASE_HIGHT = 0;

Arrow create_arrow(Vector2 center, float length, Color color) {
    return (Arrow) {
        .center = center,
        .length = length,
        .angle = 0.0f,
        .is_dragging = false,
        .color = color
    };
}

static void draw_arrow_scale(Arrow* arrow) {
    float r = arrow->length - 15.0f;

    DrawCircleSectorLines(arrow->center, r, -60, 60, 20, Fade(arrow->color, 0.3f));

    float angles[5] = { -1.047f, -0.523f, 0.0f, 0.523f, 1.047f };
    int angles_deg[5] = { -60, -30, 0, 30, 60 };

    for (int i = 0; i < 5; i++) {
        Vector2 p_start = { arrow->center.x + cosf(angles[i]) * (r - 6), arrow->center.y + sinf(angles[i]) * (r - 6) };
        Vector2 p_end = { arrow->center.x + cosf(angles[i]) * (r + 6), arrow->center.y + sinf(angles[i]) * (r + 6) };
        DrawText(TextFormat("%d", (angles_deg[i])), p_end.x + 3, p_end.y - 2, 15, RAYWHITE);

        Color mark_color = (i % 2 == 0) ? Fade(arrow->color, 0.8f) : Fade(arrow->color, 0.3f);

        DrawLineV(p_start, p_end, mark_color);
    }
}

void update_arrow(Arrow* arrow) {
    mouse = GetMousePosition();

    Vector2 arrow_end = {
        arrow->center.x + cosf(arrow->angle) * arrow->length,
        arrow->center.y + sinf(arrow->angle) * arrow->length
    };

    if (arrow->is_dragging) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
            arrow->is_dragging = false;
        }
        else {
            float dy = mouse.y - arrow->center.y;
            float dx = mouse.x - arrow->center.x;
            float target_angle = atan2f(dy, dx);

            if (target_angle > 1.57f)       target_angle = 1.047f;
            else if (target_angle < -1.57f) target_angle = -1.047f;
            else {
                if (target_angle < -1.047f) target_angle = -1.047f;
                if (target_angle > 1.047f)  target_angle = 1.047f;
            }

            arrow->angle = target_angle;
        }
    }
    else {
        if (CheckCollisionPointCircle(mouse, arrow_end, 15.0f)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                arrow->is_dragging = true;
            }
        }
    }
}

void draw_arrow(Arrow* arrow) {
    draw_arrow_scale(arrow);
    Vector2 arrow_end = {
        arrow->center.x + cosf(arrow->angle) * arrow->length,
        arrow->center.y + sinf(arrow->angle) * arrow->length
    };

    DrawLineEx(arrow->center, arrow_end, 4.0f, arrow->color);

    DrawCircleV(arrow->center, 6.0f, DARKGRAY);

    Color handle_color = arrow->is_dragging ? RED : arrow->color;
    DrawCircleV(arrow_end, 8.0f, handle_color);
}
