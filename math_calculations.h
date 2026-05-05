#pragma once

#include <stdint.h>

double distance(double x1, double y1, double x2, double y2);
double randn(void);
void calculate(bool* walls, double* F, double* F_next);
void set_cylinder_wall(bool* walls, int pos_x, int pos_y, double* F);
void init_openmp(void);
void push_liquid(double* F, uint8_t direction);
int init_lbm(void);