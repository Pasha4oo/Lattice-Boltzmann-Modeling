#include "walls.h"
#include "consts.h"

WallType wall_type = WALL_NONE;
bool* walls = NULL;
Vector2 first_wall_line_pos = { -1, -1 };
Color walls_color = { 255, 255, 150, 255 };

int WALLS_BASE_LENGTH = 0;
int WALLS_BASE_HIGHT = 60;

double distance(double x1, double y1, double x2, double y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void walls_update(void) {
	Vector2 mouse = GetMousePosition();

	mouse.x -= 50;
	mouse.y -= 25;

	if (IsKeyPressed(KEY_ESCAPE)) {
		wall_type = WALL_NONE;

		first_wall_line_pos = (Vector2){ -1, -1 };
	}

	if (wall_type == WALL_NONE 
		|| mouse.x > Nx || mouse.y > Ny || mouse.x < 0 || mouse.y < 0) { return; }

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		if (wall_type == WALL_FREE) {
			set_circle_wall(mouse, 20);
		}
		else if (wall_type == WALL_ERASER) {
			walls_eraser(mouse, 20);
		}
	}

	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { return; }
	else if (wall_type == WALL_CIRCLE) {
		set_circle_wall(mouse, 60);
	}
	else if (wall_type == WALL_LINE) {
		if (first_wall_line_pos.x == -1 && first_wall_line_pos.y == -1) {
			first_wall_line_pos = mouse;
		}
		else {
			set_line_wall(first_wall_line_pos, mouse);

			first_wall_line_pos = (Vector2){ -1, -1 };
		}
	}
	else if (wall_type == WALL_POLY) {
		
	}
}

void walls_draw(void) {
	Vector2 mouse = GetMousePosition();

	if (wall_type == WALL_CIRCLE) {
		DrawCircleV(mouse, 60, walls_color);
	}
	else if (wall_type == WALL_LINE) {
		if (first_wall_line_pos.x != -1 && first_wall_line_pos.y != -1) {
			DrawLineEx(first_wall_line_pos, mouse, 40, walls_color);
			DrawCircleV(first_wall_line_pos, 20, walls_color);
			DrawCircleV(mouse, 20, walls_color);
		}
		else {
			DrawCircleV(mouse, 20, walls_color);
		}
	}
	else if (wall_type == WALL_FREE) {
		DrawCircleV(mouse, 20, walls_color);
	}
	else if (wall_type == WALL_ERASER) {
		DrawPolyLinesEx(mouse, 36, 20, 0, 3, GRAY);
	}
}

void init_walls(void) {
	free(walls);

	walls = malloc(Ny * Nx * sizeof(bool));

	//for (int i = 0; i < Ny * Nx; i++) {
	//	if (i < Nx || i >(Ny - 1) * Nx) {
	//		walls[i] = true;
	//	}
	//	else {
	//		walls[i] = false;
	//	}
	//}

	for (int i = 0; i < Ny * Nx; i++) {
		walls[i] = false;
	}
}

void set_line_wall(Vector2 first_pos, Vector2 second_pos) {
	for (int x = 0; x < Nx; x++) {
		for (int y = 0; y < Ny; y++) {
			float dx = second_pos.x - first_pos.x;
			float dy = second_pos.y - first_pos.y;
			float t = ((x - first_pos.x) * dx + (y - first_pos.y) * dy) / (dx * dx + dy * dy);

			if (t < 0) t = 0;
			if (t > 1) t = 1;

			float proj_x = first_pos.x + t * dx;
			float proj_y = first_pos.y + t * dy;

			if (distance(proj_x, proj_y, x, y) < 20) {
				walls[y * Nx + x] = true;
			}
		}
	}
}

void set_circle_wall(Vector2 pos, int radius) {
	for (int x = 0; x < Nx; x++) {
		for (int y = 0; y < Ny; y++) {
			if (distance(pos.x, pos.y, x, y) < radius) {
				walls[y * Nx + x] = true;
				//for (int i = 0; i < NL; i++) {
				//	F[(y * Nx + x) * NL + i] = weights[i] * 1; // ρ = 1
				//}
				//[y * Nx + x] = 1.0 + 0.01 * randn();
			}
		}
	}
}

void walls_eraser(Vector2 pos, int radius) {
	for (int x = 0; x < Nx; x++) {
		for (int y = 0; y < Ny; y++) {
			if (distance(pos.x, pos.y, x, y) < radius) {
				walls[y * Nx + x] = false;
			}
		}
	}
}