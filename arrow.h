#pragma once

#include <raylib.h>
#include <math.h>
#include <stdbool.h>

typedef struct Arrow {
    Vector2 center;
    float length;
    float angle;
    bool is_dragging;
    Color color;
} Arrow;

extern const int ARROW_BASE_LENGTH;
extern const int ARROW_BASE_HIGHT;

Arrow create_arrow(Vector2 center, float length, Color color);
void update_arrow(Arrow* arrow);
void draw_arrow(Arrow* arrow);