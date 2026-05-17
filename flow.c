#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#include "flow.h"
#include "consts.h"
#include "settings.h"
#include "walls.h"

double* F = NULL;
double* F_next = NULL;

void init_F() {
	free(F);
	free(F_next);

	F = malloc(Ny * Nx * NL * sizeof(double));
	F_next = malloc(Ny * Nx * NL * sizeof(double));

	if (F == NULL || F_next == NULL) return;

	/*for (int i = 0; i < Ny * Nx * NL; i++) {
		F[i] = 1.0 + 0.01 * randn();
		F_next[i] = 0.0;
	}*/

	for (int idx = 0; idx < Ny * Nx; ++idx) {
		double rho = 1.0 + 1e-6 * randn();
		int offset = idx * NL;
		for (int dir = 0; dir < NL; ++dir) {
			F[offset + dir] = weights[dir] * rho;
		}
	}

	for (int i = 0; i < Ny * Nx * NL; ++i) {
		F_next[i] = 0.0;
	}
}

double randn(void) {
	double u1 = (double)rand() / RAND_MAX;
	double u2 = (double)rand() / RAND_MAX;

	if (u1 < 1e-9) u1 = 1e-9;

	return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

//void save_velocity_to_csv(int it, int Nx, int Ny, int NL, double* F, int* cxs, int* cys) {
//	char filename[50];
//	sprintf(filename, "velocity_%04d.csv", it/plot_every);
//	FILE* f = fopen(filename, "w");
//	if (f == NULL) return;
//
//	for (int y = 0; y < Ny; y++) {
//		for (int x = 0; x < Nx; x++) {
//			double rho = 0, ux = 0, uy = 0;
//			int cell = (y * Nx + x) * NL;
//			for (int i = 0; i < NL; i++) {
//				rho += F[cell + i];
//				ux += F[cell + i] * cxs[i];
//				uy += F[cell + i] * cys[i];
//			}
//			ux /= rho; uy /= rho;
//			double v_mag = sqrt(ux * ux + uy * uy);
//
//			fprintf(f, "%.6f%s", v_mag, (x == Nx - 1) ? "" : ";");
//		}
//		fprintf(f, "\n");
//	}
//	fclose(f);
//}

void calculate(double* F, double* F_next) {
	int x, y, i;

	// ---------- Коллизия (выполняется до стриминга) ----------
	#pragma omp parallel for private(y, x, i) collapse(2)
	for (y = 0; y < Ny; y++) {
		for (x = 0; x < Nx; x++) {
			if (walls[y * Nx + x]) continue;
			int cellOffset = (y * Nx + x) * NL;

			double rho = 0.0, ux = 0.0, uy = 0.0;
			for (i = 0; i < NL; i++) {
				double val = F[cellOffset + i];
				rho += val;
				ux += val * cxs[i];
				uy += val * cys[i];
			}
			if (rho > 1e-10) {
				ux /= rho;
				uy /= rho;
			}
			else {
				ux = 0.0; uy = 0.0;
			}

			double u_sq = ux * ux + uy * uy;
			for (i = 0; i < NL; i++) {
				double cu = cxs[i] * ux + cys[i] * uy;
				double feq = weights[i] * rho *
					(1.0 + 3.0 * cu + 4.5 * cu * cu - 1.5 * u_sq);
				F[cellOffset + i] = F[cellOffset + i] - (1.0 / tau) * (F[cellOffset + i] - feq);
			}
		}
	}

	int opp[] = { 0, 5, 6, 7, 8, 1, 2, 3, 4 };

	// ---------- Стриминг с отражением от стенок ----------
	#pragma omp parallel for private(x, y, i) collapse(2)
	for (y = 0; y < Ny; y++) {
		for (x = 0; x < Nx; x++) {
			if (walls[y * Nx + x]) continue;

			for (i = 0; i < NL; i++) {
				int next_x = x + cxs[i];
				int next_y = y + cys[i];

				if (area_type == AREA_CYCLIC) {
					next_x = (next_x + Nx) % Nx;
					next_y = (next_y + Ny) % Ny;
				}
				else if (area_type == AREA_OUTGOING) {
					next_y = (next_y + Ny) % Ny;
					if (next_x < 0 || next_x >= Nx) continue;
				}

				if (walls[next_y * Nx + next_x]) {
					F_next[(y * Nx + x) * NL + opp[i]] = F[(y * Nx + x) * NL + i];
				}
				else {
					F_next[(next_y * Nx + next_x) * NL + i] = F[(y * Nx + x) * NL + i];
				}
			}
		}
	}

	// ---------- Граничные условия (вход/выход) ----------
	if (area_type == AREA_OUTGOING) {
		// Выход (x = Nx-1): копируем из внутреннего соседа
		#pragma omp parallel for private(y, i)
		for (y = 0; y < Ny; y++) {
			if (walls[y * Nx + (Nx - 1)]) continue;
			int idx_out = (y * Nx + (Nx - 1)) * NL;
			int idx_in = (y * Nx + (Nx - 2)) * NL;
			for (i = 0; i < NL; i++) {
				F_next[idx_out + i] = F_next[idx_in + i];
			}
		}

		// Вход (x == 0): Zou-He по заданной скорости u_in = 0.07
		#pragma omp parallel for private(y, i)
		for (y = 0; y < Ny; y++) {
			if (walls[y * Nx + 0]) continue;
			int idx = (y * Nx + 0) * NL;
			double u_in = flow_speed;
			double rho = (F_next[idx + 0] + F_next[idx + 1] + F_next[idx + 5] +
				2.0 * (F_next[idx + 6] + F_next[idx + 7] + F_next[idx + 8]))
				/ (1.0 - u_in);
			for (i = 0; i < NL; i++) {
				double cu = cxs[i] * u_in;
				double u2 = u_in * u_in;
				F_next[idx + i] = weights[i] * rho *
					(1.0 + 3.0 * cu + 4.5 * cu * cu - 1.5 * u2);
			}
		}
	}

	// ---------- Копирование F_next в F для всех жидких узлов ----------
	#pragma omp parallel for private(x, y, i) collapse(2)
	for (y = 0; y < Ny; y++) {
		for (x = 0; x < Nx; x++) {
			if (walls[y * Nx + x]) continue;
			int offset = (y * Nx + x) * NL;
			for (i = 0; i < NL; i++) {
				F[offset + i] = F_next[offset + i];
			}
		}
	}
}

void init_openmp(void) {
	omp_set_num_threads(omp_get_num_procs());

	#pragma omp parallel
	{
		#pragma omp single
		printf("Number of OpenMP threads: %d\n", omp_get_num_threads());
	}
}

void push_liquid(double* F, uint8_t direction) {
	for (int y = 0; y < Ny; y++) {
		for (int x = 0; x < Nx; x++) {
			int index = (y * Nx + x) * NL + direction;
			F[index] = 0.01;	
		}
	}
}