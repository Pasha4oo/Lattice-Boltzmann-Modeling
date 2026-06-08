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
	WALL_FREE
} WallType;

typedef struct Walls {
	bool* walls;
	WallType wall_type;
	Vector2 first_wall_line_pos;
	Color walls_color;
	float brush_size;
} Walls;

extern const int WALLS_BASE_LENGTH;
extern const int WALLS_BASE_HIGHT;

double distance(double x1, double y1, double x2, double y2);
void init_walls(bool** walls);
void walls_update(Walls* ws);
void walls_draw(const Walls* ws);
void set_line_wall(Vector2 first_pos, Vector2 second_pos, Walls* ws);
void set_circle_wall(Vector2 pos, Walls* ws);
void walls_eraser(Vector2 pos, Walls* ws);
void load_walls_bmp(Walls* ws);
void load_walls_pwal(Walls* ws);
void save_walls_pwal(const Walls* ws);
void set_clogged_walls(Walls* ws);
void remove_clogged_walls(Walls* ws);