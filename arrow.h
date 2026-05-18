#pragma once

#include <raylib.h>
#include <math.h>

typedef struct {
    Vector2 center;
    float length;
    float angle;
    bool is_dragging;
    Color color;
} Arrow;

extern Arrow arrow;
extern int ARROW_BASE_LENGTH;
extern int ARROW_BASE_HIGHT;

Arrow create_arrow(Vector2 center, float length, Color color);
void update_arrow(Arrow* arrow);
void draw_arrow(Arrow* arrow);