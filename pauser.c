#include "pauser.h"

bool is_playing = true;

void switch_pauser(void) {
	if (is_playing) {
		is_playing = false;
	}
	else {
		is_playing = true;
	}
}

void draw_pauser(Vector2 pos, Vector2 size) {
    if (is_playing) {
        float bar_width = size.x * 0.35f;
        float gap = size.x * 0.30f;
        float roundness = 0.8f;

        Rectangle left_bar = { pos.x, pos.y, bar_width, size.y };
        DrawRectangleRounded(left_bar, roundness, 4, RED);

        Rectangle right_bar = { pos.x + bar_width + gap, pos.y, bar_width, size.y };
        DrawRectangleRounded(right_bar, roundness, 4, RED);
    }
    else {
        Vector2 p1 = { pos.x, pos.y };
        Vector2 p2 = { pos.x, pos.y + size.y };
        Vector2 p3 = { pos.x + size.x, pos.y + size.y / 2.0f };

        float ball_radius = size.x * 0.10f;
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
