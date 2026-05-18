#pragma once

#include <raylib.h>

typedef enum AreaType {
	AREA_CLOGGED,
	AREA_CYCLIC,
	AREA_OUTGOING
} AreaType;

extern int TAU_BASE_LENGTH;
extern int TAU_BASE_HIGHT;

extern AreaType area_type;
extern float brush_size;
extern float flow_speed;
extern double tau;

void load_all_psim();
void save_all_psim();