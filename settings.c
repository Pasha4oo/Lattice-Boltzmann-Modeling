#include "settings.h"

#include "files.h"
#include "flow.h"
#include "walls.h"

AreaType area_type = AREA_OUTGOING;
FlowDirection flow_direction = FLOW_LEFT_TO_RIGHT;
float brush_size = 10.0f;

void load_all_psim() {
	char path[260] = "";

	if (get_open_path("Select Walls PSIM Map (*.psim)\0*.psim\0All Files (*.*)\0*.*\0", "psim", path, sizeof(path))) {
		size_t skip_bytes = 0;
		load_bin(path, walls, sizeof(bool), Nx * Ny, &skip_bytes);
		load_bin(path, F, sizeof(double), Nx * Ny * NL, &skip_bytes);
		load_bin(path, &area_type, sizeof(AreaType), 1, &skip_bytes);
		load_bin(path, &flow_direction, sizeof(FlowDirection), 1, &skip_bytes);
		load_bin(path, &brush_size, sizeof(float), 1, &skip_bytes);
	}
}

void save_all_psim() {
	char path[260] = "";

	if (get_save_path("Select Walls PSIM Map (*.psim)\0*.psim\0All Files (*.*)\0*.*\0", "psim", path, sizeof(path))) {
		clear_bin(path);

		save_bin(path, walls, sizeof(bool), Nx * Ny);
		save_bin(path, F, sizeof(double), Nx * Ny * NL);
		save_bin(path, &area_type, sizeof(AreaType), 1);
		save_bin(path, &flow_direction, sizeof(FlowDirection), 1);
		save_bin(path, &brush_size, sizeof(float), 1);
	}
}
