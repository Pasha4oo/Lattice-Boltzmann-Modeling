#include "window.h"
#include "walls.h"

void update_speed_pixels(double* F, uint32_t* pixels, double max_v) {
    int y, x, i;

    #pragma omp parallel for private(y, x, i) collapse(2)
    for (y = 0; y < Ny; y++) {
        for (x = 0; x < Nx; x++) {
            if (!walls[y * Nx + x]) {
                double rho = 0, ux = 0, uy = 0;
                int cell = (y * Nx + x) * NL;
                for (i = 0; i < NL; i++) {
                    rho += F[cell + i];
                    ux += F[cell + i] * cxs[i];
                    uy += F[cell + i] * cys[i];
                }
                ux /= rho; uy /= rho;
                double v_mag = sqrt(ux * ux + uy * uy) / max_v * 1.7;

                unsigned char r, g, b;
                if (v_mag < 0.5) {
                    r = 0;
                    g = (unsigned char)(v_mag * 2 * 255);
                    b = (unsigned char)((1 - v_mag * 2) * 255);
                }
                else {
                    r = (unsigned char)((v_mag - 0.5) * 2 * 255);
                    g = (unsigned char)((1 - (v_mag - 0.5) * 2) * 110);
                    b = 40;
                }

                pixels[y * Nx + x] = (255 << 24) | (r << 16) | (g << 8) | b;
            }
            else {
                pixels[y * Nx + x] = (walls_color.a << 24) | (walls_color.b << 16) | (walls_color.g << 8) | walls_color.r;
            }
        }
    }
}

void update_max_v(double* max_v, double* F) {
    double current_max = 0.0f;
    int x, y, i;
    for (y = 0; y < Ny; y++) {
        for (x = 0; x < Nx; x++) {
            double rho = 0, ux = 0, uy = 0;
            int cell = (y * Nx + x) * NL;
            for (i = 0; i < NL; i++) {
                rho += F[cell + i];
                ux += F[cell + i] * cxs[i];
                uy += F[cell + i] * cys[i];
            }
            ux /= rho; uy /= rho;
            double v = sqrt(ux * ux + uy * uy);
            if (v > current_max) current_max = v;
        }
    }

    *max_v = 0.9 * (*max_v) + 0.1 * current_max; //0.9
    if (*max_v < 1e-12) *max_v = 1e-12;
}