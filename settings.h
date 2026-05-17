#pragma once

#include <raylib.h>

typedef enum AreaType {
	AREA_CLOGGED,
	AREA_CYCLIC,
	AREA_OUTGOING
} AreaType;

typedef enum FlowDirection {
	FLOW_LEFT_TO_RIGHT,
	FLOW_RIGHT_TO_LEFT,
	FLOW_UP_TO_DOWN,
	FLOW_DOWN_TO_UP
} FlowDirection;

extern AreaType area_type;
extern FlowDirection flow_direction;
extern float brush_size;
extern float flow_speed;

void load_all_psim();
void save_all_psim();