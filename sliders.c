#include "sliders.h"

#include "settings.h"

static void recalculate_slider_handle(Slider* slider) {
    float handle_width = slider->rect.width + 8.0f;
    float handle_height = 16.0f;

    slider->handle_rect.x = slider->rect.x - (handle_width - slider->rect.width) / 2.0f;
    slider->handle_rect.y = slider->rect.y + ((1.0f - slider->value) * slider->rect.height) - (handle_height / 2.0f);
    slider->handle_rect.width = handle_width;
    slider->handle_rect.height = handle_height;
}

Slider create_slider(UISliders* ui_sliders, Rectangle rect, Color bar_color, Color handle_color, SliderID id, float value) {
	Slider slider = (Slider)
	{
		.rect = rect,
		.bar_color = bar_color,
		.base_color = handle_color,
        .render_color = handle_color,
        .target_color = handle_color,
		.is_dragging = false,
		.value = value,
        .id = id
	};

    recalculate_slider_handle(&slider);

	if (ui_sliders->counter < MAX_SLIDERS) {
		ui_sliders->sliders[ui_sliders->counter++] = slider;
	}

	return slider;
}

void update_slider(Slider* slider) {
    Vector2 mouse = GetMousePosition();

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

void ui_sliders_update(UISliders* ui_sliders, Settings* settings, Walls* walls) {
    for (int i = 0; i < ui_sliders->counter; i++) {
        update_slider(&ui_sliders->sliders[i]);

        if (ui_sliders->sliders[i].is_dragging) {
            switch (ui_sliders->sliders[i].id) {
                case SLIDER_BRUSH:           walls->brush_size = ui_sliders->sliders[i].value * 20 + 4; break;
                case SLIDER_SPEED:           settings->flow_speed = ui_sliders->sliders[i].value * 0.14; break;
                case SLIDER_TAU:           settings->tau = ui_sliders->sliders[i].value * 1.2 + 0.5001; break;

                }
        }
    }
}

void ui_sliders_draw(UISliders* ui_sliders) {
    for (int i = 0; i < ui_sliders->counter; i++) {
        draw_slider(&ui_sliders->sliders[i]);
    }
}

void init_sliders(UISliders* ui, Walls* walls, Settings* settings) {
    create_slider(ui, (Rectangle) { 360 + WALLS_BASE_LENGTH, Ny + 65 + FILES_BASE_HIGHT, 15, 110 },
        ORANGE, DARKBROWN, SLIDER_BRUSH, 0.5f);
    create_slider(ui, (Rectangle) { 275 + FILES_BASE_LENGTH, Ny + 65 + FILES_BASE_HIGHT, 15, 110 },
        ORANGE, DARKBROWN, SLIDER_SPEED, 0.5f);
    create_slider(ui, (Rectangle) { 140 + Nx + TAU_BASE_LENGTH, 155 + TAU_BASE_HIGHT, 15, 110 },
        ORANGE, DARKBROWN, SLIDER_TAU, 0.05825f);
}