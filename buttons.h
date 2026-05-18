#pragma once

#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_BUTTONS 32

typedef enum ButtonID {
	BUTTON_PAUSER,
	BUTTON_RESTART_LIQUID,
	BUTTON_WALL_CIRCLE,
	BUTTON_WALL_LINE,
	BUTTON_WALL_FREE,
	BUTTON_WALL_NONE,
	BUTTON_WALL_ERASER,
	BUTTON_WALL_CLEAR,
	BUTTON_LOAD_BMP,
	BUTTON_LOAD_WALL,
	BUTTON_SAVE_WALL,
	BUTTON_LOAD_ALL,
	BUTTON_SAVE_ALL,
	BUTTON_AREA_CLOGGED,
	BUTTON_AREA_CYCLIC,
	BUTTON_AREA_OUTGOING
} ButtonID;

typedef struct Button {
	Rectangle rect;
	float roundness;
	Color base_color;
	Color render_color;
	Color target;
	const char* text;
	Vector2 text_pos;
	float text_width;
	bool pressed;
	ButtonID id;
	float press_timer;
} Button;

typedef struct UIButtons {
	Button buttons[MAX_BUTTONS];
	int counter;
} UIButtons;

extern UIButtons ui_buttons;

Button create_button(Rectangle rect, float rect_roundness, Color rect_color, const char* text, Color text_color, ButtonID id);

void update_button(Button* button);
void draw_button(Button* button);
void ui_buttons_update(void);
void ui_buttons_draw(void);