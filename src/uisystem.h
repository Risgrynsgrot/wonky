#pragma once
#include "raylib.h"
#include <stdbool.h>

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

typedef int ui_text_id;
typedef int ui_button_id;

typedef struct ui_text {
	char text[MAX_TEXT_LENGTH];
	Vector2 position;
	int size;
	Color color;
	int priority;
	bool visible;
	Font font;
	ui_text_id id;
} ui_text_t;

#define MAX_TEXTS 100

typedef struct ui_text_pool {
	ui_text_t texts[MAX_TEXTS];
	int text_count;
} ui_text_pool_t;

ui_text_id ui_add_text(ui_text_pool_t* text_pool, ui_text_t* text);
void ui_draw_texts(ui_text_pool_t* text_pool);

typedef struct ui_button {
	ui_text_id text;
	event_id onClickEventId;
	float offsetX;
	float offsetY;
	Color color;
	int priority;
	bool visible;
	bool pressed;
} ui_button_t;

#define MAX_BUTTONS 10

typedef struct ui_button_list {
	ui_button_t buttons[MAX_BUTTONS];
	int button_count;
} ui_button_list_t;
