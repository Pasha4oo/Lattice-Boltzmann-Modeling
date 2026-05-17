#include "sliders.h"
#include "settings.h"

UISliders ui_sliders = { 0 };
static Vector2 mouse;

static void recalculate_slider_handle(Slider* slider) {
    float handle_width = slider->rect.width + 8.0f;
    float handle_height = 16.0f;

    slider->handle_rect.x = slider->rect.x - (handle_width - slider->rect.width) / 2.0f;
    slider->handle_rect.y = slider->rect.y + ((1.0f - slider->value) * slider->rect.height) - (handle_height / 2.0f);
    slider->handle_rect.width = handle_width;
    slider->handle_rect.height = handle_height;
}

Slider create_slider(Rectangle rect, Color bar_color, Color handle_color, SliderID id) {
	Slider slider = (Slider)
	{
		.rect = rect,
		.bar_color = bar_color,
		.base_color = handle_color,
        .render_color = handle_color,
        .target_color = handle_color,
		.is_dragging = false,
		.value = 0.5f,
        .id = id
	};

    recalculate_slider_handle(&slider);

	if (ui_sliders.counter < MAX_SLIDERS) {
		ui_sliders.sliders[ui_sliders.counter++] = slider;
	}

	return slider;
}

void update_slider(Slider* slider) {
    if (slider->is_dragging) {
        slider->target_color = Fade(slider->base_color, 0.4f);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
            slider->is_dragging = false;
        }
        else {
            slider->value = 1.0f - ((mouse.y - slider->rect.y) / slider->rect.height);

            if (slider->value < 0.0f) { slider->value = 0.0f; }
            if (slider->value > 1.0f) { slider->value = 1.0f; }

            recalculate_slider_handle(slider);
        }
    }
    else if (CheckCollisionPointRec(mouse, slider->rect) || CheckCollisionPointRec(mouse, slider->handle_rect)) {
        slider->target_color = Fade(slider->base_color, 0.8f);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            slider->is_dragging = true;
            slider->render_color = Fade(slider->base_color, 0.4f);
        }
    }
    else {
        slider->target_color = slider->base_color;
    }

    float t = slider->is_dragging ? 0.05f : 0.15f;
    slider->render_color = ColorLerp(slider->render_color, slider->target_color, t);
}

void draw_slider(Slider* slider) {
    DrawRectangleRounded(slider->rect, 0.5f, 4, slider->bar_color);

    Rectangle fill_rect = slider->rect;
    fill_rect.y = slider->handle_rect.y + (slider->handle_rect.height / 2.0f);
    fill_rect.height = (slider->rect.y + slider->rect.height) - fill_rect.y;
    DrawRectangleRounded(fill_rect, 0.5f, 4, slider->base_color);

    DrawRectangleRounded(slider->handle_rect, 0.4f, 4, slider->render_color);
}

void ui_sliders_update(void) {
    mouse = GetMousePosition();

    for (int i = 0; i < ui_sliders.counter; i++) {
        update_slider(&ui_sliders.sliders[i]);

        if (ui_sliders.sliders[i].is_dragging) {
            switch (ui_sliders.sliders[i].id) {
                case SLIDER_BRUSH:           brush_size = ui_sliders.sliders[i].value * 20 + 4; break;
                case SLIDER_SPEED:           flow_speed = ui_sliders.sliders[i].value * 0.14; break;

                }
        }
    }
}

void ui_sliders_draw(void) {
    for (int i = 0; i < ui_sliders.counter; i++) {
        draw_slider(&ui_sliders.sliders[i]);
    }
}