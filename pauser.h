#pragma once

#include <stdbool.h>
#include <raylib.h>

extern bool is_playing;

void switch_pauser(void);
void draw_pauser(Vector2 pos, Vector2 size);