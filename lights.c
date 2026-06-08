#include "lights.h"

void enable_light(LightSystem* light_system, int enable_index) {
	if (enable_index >= 0 && enable_index <= light_system->lights_count) {
		light_system->enabled_index = enable_index;
	}
}

void create_light(LightSystem* light_system, Vector2 pos) {
	Light* new_lights = realloc(light_system->lights, (light_system->lights_count + 1) * sizeof(Light));

	if (new_lights) {
		light_system->lights = new_lights;
		light_system->lights[light_system->lights_count].pos = pos;
		light_system->lights_count += 1;
	}
}

void ui_lights_draw(LightSystem* light_system) {
	for (int i = 0; i < light_system->lights_count; i++) {
		if (i == light_system->enabled_index) {
			DrawCircleV(light_system->lights[i].pos, 8.0f * 1.4f, Fade(GREEN, 0.3f));
			DrawCircleV(light_system->lights[i].pos, 8.0f, GREEN);
			DrawCircleV((Vector2){ light_system->lights[i].pos.x - 2, light_system->lights[i].pos.y - 2 },
				8.0f * 0.3f, Fade(WHITE, 0.7f));
		}
		else {
			DrawCircleV(light_system->lights[i].pos, 8.0f, DARKGRAY);
			DrawCircleV((Vector2) { light_system->lights[i].pos.x - 2, light_system->lights[i].pos.y - 2 },
				8.0f * 0.2f, Fade(WHITE, 0.3f));
		}
	}
}