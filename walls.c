#include "walls.h"
#include "consts.h"
#include "files.h"
#include "settings.h"

const int WALLS_BASE_LENGTH = 0;
const int WALLS_BASE_HIGHT = 60;

double distance(double x1, double y1, double x2, double y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void walls_update(Walls* ws) {
	Vector2 mouse = GetMousePosition();
	mouse.x -= 50;
	mouse.y -= 25;

	if (IsKeyPressed(KEY_ESCAPE)) {
		ws->wall_type = WALL_NONE;
		ws->first_wall_line_pos = (Vector2){ -1, -1 };
	}

	if (ws->wall_type == WALL_NONE
		|| mouse.x > Nx || mouse.y > Ny || mouse.x < 0 || mouse.y < 0) {
		return;
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		if (ws->wall_type == WALL_FREE) {
			set_circle_wall(mouse, ws);
		}
		else if (ws->wall_type == WALL_ERASER) {
			walls_eraser(mouse, ws);
		}
	}

	if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { return; }
	else if (ws->wall_type == WALL_CIRCLE) {
		set_circle_wall(mouse, ws);
	}
	else if (ws->wall_type == WALL_LINE) {
		if (ws->first_wall_line_pos.x == -1 && ws->first_wall_line_pos.y == -1) {
			ws->first_wall_line_pos = mouse;
		}
		else {
			set_line_wall(ws->first_wall_line_pos, mouse, ws);
			ws->first_wall_line_pos = (Vector2){ -1, -1 };
		}
	}
}

void walls_draw(const Walls* ws) {
	Vector2 mouse = GetMousePosition();

	if (ws->wall_type == WALL_CIRCLE) {
		DrawCircleV(mouse, ws->brush_size, ws->walls_color);
	}
	else if (ws->wall_type == WALL_LINE) {
		if (ws->first_wall_line_pos.x != -1 && ws->first_wall_line_pos.y != -1) {
			Vector2 first_screen = { ws->first_wall_line_pos.x + 50, ws->first_wall_line_pos.y + 25 };
			DrawLineEx(first_screen, mouse, ws->brush_size * 2, ws->walls_color);
			DrawCircleV(first_screen, ws->brush_size, ws->walls_color);
			DrawCircleV(mouse, ws->brush_size, ws->walls_color);
		}
		else {
			DrawCircleV(mouse, ws->brush_size, ws->walls_color);
		}
	}
	else if (ws->wall_type == WALL_FREE) {
		DrawCircleV(mouse, ws->brush_size, ws->walls_color);
	}
	else if (ws->wall_type == WALL_ERASER) {
		DrawPolyLinesEx(mouse, 36, ws->brush_size, 0, 3, GRAY);
	}
}

void init_walls(bool** walls) {
	free(*walls);

	*walls = malloc(Ny * Nx * sizeof(bool));

	//for (int i = 0; i < Ny * Nx; i++) {
	//	if (i < Nx || i >(Ny - 1) * Nx) {
	//		walls[i] = true;
	//	}
	//	else {
	//		walls[i] = false;
	//	}
	//}

	for (int i = 0; i < Ny * Nx; i++) {
		(*walls)[i] = false;
	}
}

void set_line_wall(Vector2 first_pos, Vector2 second_pos, Walls* ws) {
	for (int x = 0; x < Nx; x++) {
		for (int y = 0; y < Ny; y++) {
			float dx = second_pos.x - first_pos.x;
			float dy = second_pos.y - first_pos.y;
			float t = ((x - first_pos.x) * dx + (y - first_pos.y) * dy) / (dx * dx + dy * dy);

			if (t < 0) t = 0;
			if (t > 1) t = 1;

			float proj_x = first_pos.x + t * dx;
			float proj_y = first_pos.y + t * dy;

			if (distance(proj_x, proj_y, x, y) < ws->brush_size) {
				ws->walls[y * Nx + x] = true;
			}
		}
	}
}

void set_circle_wall(Vector2 pos, Walls* ws) {
	for (int x = 0; x < Nx; x++) {
		for (int y = 0; y < Ny; y++) {
			if (distance(pos.x, pos.y, x, y) < ws->brush_size) {
				ws->walls[y * Nx + x] = true;
				//for (int i = 0; i < NL; i++) {
				//	F[(y * Nx + x) * NL + i] = weights[i] * 1; // ρ = 1
				//}
				//[y * Nx + x] = 1.0 + 0.01 * randn();
			}
		}
	}
}

void walls_eraser(Vector2 pos, Walls* ws) {
	for (int x = 0; x < Nx; x++) {
		for (int y = 0; y < Ny; y++) {
			if (distance(pos.x, pos.y, x, y) < ws->brush_size) {
				ws->walls[y * Nx + x] = false;
			}
		}
	}
}

void load_walls_bmp(Walls* ws) {
	char path[260] = "";

	if (get_open_path("Select Walls BMP Map (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0", "bmp", path, sizeof(path))) {
		load_bmp(path, ws->walls, Nx * Ny);
	}
}

void load_walls_pwal(Walls* ws) {
	char path[260] = "";

	if (get_open_path("Select Walls PWAL Map (*.pwal)\0*.pwal\0Select Walls PSIM Map (*.psim)\0*.psim\0All Files (*.*)\0*.*\0", "pwal", path, sizeof(path))) {
		size_t skip_bytes = 0;
		load_bin(path, ws->walls, sizeof(bool), Nx * Ny, &skip_bytes);
	}
}

void save_walls_pwal(const Walls* ws) {
	char path[260] = "";

	if (get_save_path("Select Walls PWAL Map (*.pwal)\0*.pwal\0All Files (*.*)\0*.*\0", "pwal", path, sizeof(path))) {
		clear_bin(path);
		save_bin(path, ws->walls, sizeof(bool), Nx * Ny);
	}
}

void set_clogged_walls(Walls* ws) {
    for (int x = 0; x < Nx; x++) {
        ws->walls[0 * Nx + x] = true;
        ws->walls[(Ny - 1) * Nx + x] = true;
    }

    for (int y = 0; y < Ny; y++) {
        ws->walls[y * Nx + 0] = true;
        ws->walls[y * Nx + (Nx - 1)] = true;
    }
}

void remove_clogged_walls(Walls* ws) {
    for (int x = 0; x < Nx; x++) {
        ws->walls[0 * Nx + x] = false;
        ws->walls[(Ny - 1) * Nx + x] = false;
    }

    for (int y = 0; y < Ny; y++) {
        ws->walls[y * Nx + 0] = false;
        ws->walls[y * Nx + (Nx - 1)] = false;
    }
}