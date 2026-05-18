#pragma once

#include <raylib.h>

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

extern UISliders ui_sliders;

Slider create_slider(Rectangle rect, Color bar_color, Color handle_color, SliderID id, float value);

void update_slider(Slider* slider);
void draw_slider(Slider* slider);
void ui_sliders_update(void);
void ui_sliders_draw(void);