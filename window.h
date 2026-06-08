#pragma once

#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

#include "consts.h"
#include "flow.h"

void update_speed_pixels(LBM* lbm, Walls* ws, uint32_t* pixels);
void update_max_v(LBM* lbm);