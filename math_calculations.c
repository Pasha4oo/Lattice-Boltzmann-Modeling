#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#include "math_calculations.h"
#include "consts.h"
#include "walls.h"

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
	#pragma omp parallel for private(x, y, i) collapse(2)
	for (y = 0; y < Ny; y++) {
		for (x = 0; x < Nx; x++) {
			for (i = 0; i < NL; i++) {
				int next_x = (x + cxs[i]);
				/*int next_y = y + cys[i];*/
				int next_y = (y + cys[i] + Ny) % Ny; //Closed System

				if (next_x >= 0 && next_x < Nx) {
					F_next[(next_y * Nx + next_x) * NL + i] = F[(y * Nx + x) * NL + i];
				}

				/*if (next_x >= 0 && next_x < Nx && next_y >= 0 && next_y < Ny) {
					F_next[(next_y * Nx + next_x) * NL + i] = F[(y * Nx + x) * NL + i];
				}*/

				//int next_x = x + cxs[i];
				//int next_y = y + cys[i];

				//if (next_x < 0 || next_x >= Nx) continue;

				if (next_x >= 0 && next_x < Nx && next_y >= 0 && next_y < Ny) {
					F_next[(next_y * Nx + next_x) * NL + i] = F[(y * Nx + x) * NL + i];
				}
			}
		}
	}

	int opp[] = { 0, 5, 6, 7, 8, 1, 2, 3, 4 };

	#pragma omp parallel for private(y, x, i) collapse(2)
	for (y = 0; y < Ny; y++) {
		for (x = 0; x < Nx; x++) {
			if (walls[y * Nx + x]) {
				for (i = 0; i < NL; i++) {
					F[(y * Nx + x) * NL + i] = F_next[(y * Nx + x) * NL + opp[i]];
					//F[y * Nx + x] = 0;
				}
			}
			/*else if (x == Nx - 1) {
				int idx_left = (y * Nx + (x - 1)) * NL;
				for (i = 0; i < NL; i++) {
					F[(y * Nx + x) * NL + i] = F_next[idx_left + i];
				}
			}
			else if (x == 0) {
				double u_in = 0.07;
				int idx = (y * Nx + x) * NL;

				double rho = (F_next[idx + 0] + F_next[idx + 2] + F_next[idx + 4] +
					2.0 * (F_next[idx + 3] + F_next[idx + 6] + F_next[idx + 7])) / (1.0 - u_in);
			}*/
			else if (x == 0) {
				double u_in = 0.07;              
				int idx = (y * Nx + 0) * NL;

				double rho = (F_next[idx + 0] + F_next[idx + 2] + F_next[idx + 4] +
					2.0 * (F_next[idx + 1] + F_next[idx + 5] + F_next[idx + 8]))
					/ (1.0 - u_in);

				for (i = 0; i < NL; i++) {
					double cu = cxs[i] * u_in;   
					double u2 = u_in * u_in;
					F[idx + i] = weights[i] * rho *
						(1.0 + 3.0 * cu + 4.5 * cu * cu - 1.5 * u2);
				}
			}
			else if (x == Nx - 1) {
				int idx_left = (y * Nx + (Nx - 2)) * NL;
				for (i = 0; i < NL; i++) {
					F[(y * Nx + (Nx - 1)) * NL + i] = F_next[idx_left + i];
				}
			}
			/*else if (y == Ny - 1) {
				int idx_below = ((Ny - 2) * Nx + x) * NL;
				for (i = 0; i < NL; i++) {
					F[((Ny - 1) * Nx + x) * NL + i] = F_next[idx_below + i];
				}
			}
			else if (y == 0) {
				int idx_above = (1 * Nx + x) * NL;
				for (i = 0; i < NL; i++) {
					F[(0 * Nx + x) * NL + i] = F_next[idx_above + i];
				}
			}*/
			else {
				for (i = 0; i < NL; i++) {
					F[(y * Nx + x) * NL + i] = F_next[(y * Nx + x) * NL + i];
				}
			}
		}
	}

	#pragma omp parallel for private(y, x, i) collapse(2)
	for (y = 0; y < Ny; y++) {
		for (x = 0; x < Nx; x++) {
			int cellOffset = (y * Nx + x) * NL;

			if (walls[y * Nx + x]) continue;

			double rho = 0, ux = 0, uy = 0;
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
				ux = 0; uy = 0;
			}

			//ux = ux / rho + 5e-4 * tau / rho;
			//uy = uy / rho + 0.0 * tau / rho;

			double u_sq = ux * ux + uy * uy;

			for (i = 0; i < NL; i++) {
				double cu = cxs[i] * ux + cys[i] * uy;

				double feq = weights[i] * rho * (
					1.0 + 3.0 * cu + 4.5 * (cu * cu) - 1.5 * u_sq
					);

				F[cellOffset + i] = F[cellOffset + i] - (1.0 / tau) * (F[cellOffset + i] - feq);
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