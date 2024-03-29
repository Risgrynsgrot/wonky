#pragma once
#include "raylib.h"
#include <stdbool.h>

void ui_draw_buttons(void);

typedef enum ui_event_trigger {
	UI_EVENT_INSTANT,
	UI_EVENT_NEXT_FRAME
} ui_event_trigger_e;

typedef int event_id;

typedef struct ui_event {
	void (*function)(void);
	ui_event_trigger_e trigger;
	event_id id;
} ui_event_t;

#define MAX_EVENTS 256

typedef struct ui_event_handler {
	ui_event_t events[MAX_EVENTS];
	int trigger_next_frame[MAX_EVENTS];
	int event_count;
} ui_event_handler_t;

event_id ui_create_event(ui_event_handler_t* event_handler,
						 const ui_event_trigger_e eventTrigger,
						 void (*eventFunction)(void));
ui_event_handler_t create_event_handler(void);
void ui_trigger_event(ui_event_handler_t* event_handler, const int id);

#define MAX_TEXT_LENGTH 256 //probably a bit small

typedef struct ui_text {
	char text[MAX_TEXT_LENGTH];
	int size;
	float offsetX;
	float offsetY;
	Color color;
	int priority;
	bool visible;
} ui_text_t;

typedef struct ui_button {
	ui_text_t* text;
	int onClickEventId;
	float offsetX;
	float offsetY;
	Color color;
	int priority;
	bool visible;
	bool pressed;
} ui_button_t;
