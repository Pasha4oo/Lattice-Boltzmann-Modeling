#pragma once

#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "buttons_callback.h"
#include "walls.h"
#include "flow.h"
#include "settings.h"
#include "lights.h"
#include "pauser.h"
#include "files.h"
#include "arrow.h"

#define MAX_BUTTONS 32

typedef void (*ButtonCallback)(void* user_data);

typedef struct Button {
	Rectangle rect;
	float roundness;
	Color base_color;
	Color render_color;
	Color target;
	const char* text;
	Vector2 text_pos;
	float text_width;
	float press_timer;

	ButtonCallback callback;
	void* callback_data;
} Button;

typedef struct UIButtons {
	Button buttons[MAX_BUTTONS];
	int counter;
} UIButtons;

Button create_button(UIButtons* ui_buttons, Rectangle rect, float rect_roundness, Color rect_color, 
	const char* text, Color text_color, ButtonCallback callback, void* callback_data);

void update_button(Button* button);
void draw_button(Button* button);
void ui_buttons_update(UIButtons* ui_buttons);
void ui_buttons_draw(const UIButtons* ui_buttons);

void init_buttons(UIButtons* ui, Walls* ws, LBM* lbm, Pauser* pauser, Arrow* arrow, Settings* settings, LightSystem* light_system);;