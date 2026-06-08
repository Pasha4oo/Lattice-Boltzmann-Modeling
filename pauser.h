#pragma once

#include <stdbool.h>
#include <raylib.h>

typedef struct {
    bool is_playing;
    Vector2 pos;
    Vector2 size;
} Pauser;

void switch_pauser(Pauser* p);
void draw_pauser(const Pauser* p);