#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include <raylib.h>

#include "consts.h"

typedef enum WallType {
	WALL_NONE,
	WALL_ERASER,
	WALL_CIRCLE,
	WALL_LINE,
	WALL_FREE,
	WALL_POLY
} WallType;

extern bool* walls;

extern WallType wall_type;
extern Color walls_color;

double distance(double x1, double y1, double x2, double y2);
void init_walls(void);
void walls_update(void);
void walls_draw(void);
void set_line_wall(Vector2 first_pos, Vector2 second_pos);
void set_circle_wall(Vector2 pos, int radius);
void walls_eraser(Vector2 pos, int radius);