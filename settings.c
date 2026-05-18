#include "settings.h"

#include "files.h"
#include "flow.h"
#include "walls.h"
#include "lights.h"
#include "arrow.h"

int TAU_BASE_LENGTH = 100;
int TAU_BASE_HIGHT = 100;

AreaType area_type = AREA_OUTGOING;
float brush_size = 14.0f;
float flow_speed = 0.07f;
double tau = .57; //0.53

void load_all_psim() {
	char path[260] = "";

	if (get_open_path("Select Walls PSIM Map (*.psim)\0*.psim\0All Files (*.*)\0*.*\0", "psim", path, sizeof(path))) {
		size_t skip_bytes = 0;
		load_bin(path, walls, sizeof(bool), Nx * Ny, &skip_bytes);
		load_bin(path, F, sizeof(double), Nx * Ny * NL, &skip_bytes);
		load_bin(path, &area_type, sizeof(AreaType), 1, &skip_bytes);
		load_bin(path, &brush_size, sizeof(float), 1, &skip_bytes);
		load_bin(path, &flow_speed, sizeof(float), 1, &skip_bytes);
		load_bin(path, &light_system.enabled_index, sizeof(int), 1, &skip_bytes);
		load_bin(path, &arrow.angle, sizeof(float), 1, &skip_bytes);
		load_bin(path, &tau, sizeof(double), 1, &skip_bytes);
	}
}

void save_all_psim() {
	char path[260] = "";

	if (get_save_path("Select Walls PSIM Map (*.psim)\0*.psim\0All Files (*.*)\0*.*\0", "psim", path, sizeof(path))) {
		clear_bin(path);

		save_bin(path, walls, sizeof(bool), Nx * Ny);
		save_bin(path, F, sizeof(double), Nx * Ny * NL);
		save_bin(path, &area_type, sizeof(AreaType), 1);
		save_bin(path, &brush_size, sizeof(float), 1);
		save_bin(path, &flow_speed, sizeof(float), 1);
		save_bin(path, &light_system.enabled_index, sizeof(int), 1);
		save_bin(path, &arrow.angle, sizeof(float), 1);
		save_bin(path, &tau, sizeof(double), 1);
	}
}
