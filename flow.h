#pragma once

#include <stdint.h>

extern double* F;
extern double* F_next;
extern double max_v;

void init_F();
double randn(void);
void calculate(double* F, double* F_next);
void init_openmp(void);
void push_liquid(double* F, uint8_t direction);
int init_lbm(void);