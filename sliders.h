#pragma once

#include <raylib.h>

#include "files.h"
#include "flow.h"
#include "walls.h"
#include "lights.h"
#include "arrow.h"

#define MAX_SLIDERS 32

typedef enum SliderID {
    SLIDER_BRUSH,
    SLIDER_SPEED,
    SLIDER_TAU
} SliderID;

typedef struct Slider {
    Rectangle rect;
    Rectangle handle_rect;
    float value;
    Color bar_color;
    Color base_color;
    Color render_color;
    Color target_color;
    bool is_dragging;
    SliderID id;
} Slider;

typedef struct UISliders {
    Slider sliders[MAX_SLIDERS];
	int counter;
} UISliders;

Slider create_slider(UISliders* ui_sliders, Rectangle rect, Color bar_color, Color handle_color, SliderID id, float value);

void update_slider(Slider* slider);
void draw_slider(Slider* slider);
void ui_sliders_update(UISliders* ui_sliders, Settings* settings, Walls* walls);
void ui_sliders_draw(UISliders* ui_sliders);