#pragma once

#include <stdint.h>

#include "walls.h"
#include "consts.h"
#include "settings.h"
#include "arrow.h"

typedef struct LBM {
    double* F;
    double* F_next;
    double max_v;
} LBM;

void init_lbm(LBM* lbm);
double randn(void);
void calculate(LBM* lbm, const Walls* ws, const Settings* settings, const Arrow* arrow);
void init_openmp(void);
void push_liquid(LBM* lbm, uint8_t direction);