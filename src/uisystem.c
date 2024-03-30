#include "uisystem.h"

void ui_draw_buttons(void) {
}

event_id ui_create_event(ui_event_handler_t* event_handler,
						 const ui_event_trigger_e event_trigger,
						 void (*event_function)(void)) {

	const int id					   = event_handler->event_count;
	event_handler->events[id].id	   = id;
	event_handler->events[id].trigger  = event_trigger;
	event_handler->events[id].function = event_function;
	event_handler->event_count++;
	return id;
}

ui_event_handler_t create_event_handler(void) {
	ui_event_handler_t result;
	result.event_count = 0;
	return result;
}

void ui_trigger_event(ui_event_handler_t* event_handler, const int id) {
	const ui_event_t* event = &event_handler->events[id];
	switch(event->trigger) {
	case UI_EVENT_INSTANT:
		event->function();
		return;
	case UI_EVENT_NEXT_FRAME:
		event_handler->trigger_next_frame[id] = true;
		return;
	}
}

ui_text_id ui_add_text(ui_text_pool_t* text_pool, ui_text_t* text) {
	int id				 = text_pool->text_count;
	text_pool->texts[id] = *text;
	text_pool->text_count++;
	return id;
}

void ui_draw_texts(ui_text_pool_t* text_pool) {
	for(int i = 0; i < text_pool->text_count; i++) {
		const ui_text_t* text = &text_pool->texts[i];
		if(!text->visible) {
			continue;
		}
		DrawTextEx(
			text->font, text->text, text->position, text->size, 0, text->color);
	}
}
