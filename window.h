#pragma once

#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#include "consts.h"

void update_speed_pixels(double* F, uint32_t* pixels, double max_v);
void update_max_v(double* max_v, double* F);