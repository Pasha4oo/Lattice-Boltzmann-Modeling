#include "pauser.h"

void switch_pauser(Pauser* pauser) {
	if (pauser->is_playing) {
        pauser->is_playing = false;
	}
	else {
        pauser->is_playing = true;
	}
}

void draw_pauser(const Pauser* pauser) {
    if (pauser->is_playing) {
        float bar_width = pauser->size.x * 0.35f;
        float gap = pauser->size.x * 0.30f;
        float roundness = 0.8f;

        Rectangle left_bar = { pauser->pos.x, pauser->pos.y, bar_width, pauser->size.y };
        DrawRectangleRounded(left_bar, roundness, 4, RED);

        Rectangle right_bar = { pauser->pos.x + bar_width + gap, pauser->pos.y, bar_width, pauser->size.y };
        DrawRectangleRounded(right_bar, roundness, 4, RED);
    }
    else {
        Vector2 p1 = { pauser->pos.x, pauser->pos.y };
        Vector2 p2 = { pauser->pos.x, pauser->pos.y + pauser->size.y };
        Vector2 p3 = { pauser->pos.x + pauser->size.x, pauser->pos.y + pauser->size.y / 2.0f };

        float ball_radius = pauser->size.x * 0.10f;
        Color color = (Color){ 0, 185, 39, 255 };

        DrawCircleV(p1, ball_radius, color);
        DrawCircleV(p2, ball_radius, color);
        DrawCircleV(p3, ball_radius, color);

        DrawLineEx(p1, p2, ball_radius * 2.0f, color);
        DrawLineEx(p2, p3, ball_radius * 2.0f, color);
        DrawLineEx(p3, p1, ball_radius * 2.0f, color);

        DrawTriangle(p1, p2, p3, color);
    }
}