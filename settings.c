#include "settings.h"

#include <locale.h>

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
bool fixate = false;

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
		load_bin(path, &fixate, sizeof(bool), 1, &skip_bytes);
		load_bin(path, &max_v, sizeof(double), 1, &skip_bytes);
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
		save_bin(path, &fixate, sizeof(bool), 1);
		save_bin(path, &max_v, sizeof(double), 1);
	}
}

void export_velocity_csv() {
	char path[260] = "";

	if (!get_save_path("Select CSV File (*.csv)\0*.csv\0All Files (*.*)\0*.*\0", "csv", path, sizeof(path))) return;
	clear_bin(path);

	FILE* f = fopen(path, "w");
	if (f == NULL) return;
	char* old_locale = setlocale(LC_NUMERIC, "ru_RU.UTF-8");

	for (int y = 0; y < Ny; y++) {
		for (int x = 0; x < Nx; x++) {
			double rho = 0, ux = 0, uy = 0;
			int cell = (y * Nx + x) * NL;
			for (int i = 0; i < NL; i++) {
				rho += F[cell + i];
				ux += F[cell + i] * cxs[i];
				uy += F[cell + i] * cys[i];
			}
			ux /= rho; uy /= rho;
			double v_mag = sqrt(ux * ux + uy * uy);

			fprintf(f, "%.6f%s", v_mag, (x == Nx - 1) ? "" : ";");
		}
		fprintf(f, "\n");
	}
	fclose(f);
}