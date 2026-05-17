#pragma once

#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Light {
	Vector2 pos;
} Light;

typedef struct LightSystem {
	Light* lights;
	int enabled_index;
	int lights_count;
} LightSystem;

extern LightSystem light_system;

void enable_light(LightSystem* light_system, int enable_index);
void create_light(LightSystem* light_system, Vector2 pos);
void ui_lights_draw(LightSystem* light_system);