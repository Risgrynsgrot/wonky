#pragma once
#include "entity.h"
#include <raylib.h>
#include <stdbool.h>

typedef struct comp_position {
	Vector2 value;
} comp_position_t;

typedef struct comp_rotation {
	float angle;
} comp_rotation_t;

typedef struct comp_velocity {
	Vector2 value;
} comp_velocity_t;

typedef struct comp_input {
	int input_id;
	Vector2 direction;
	bool interact;
	bool open_inventory;
} comp_input_t;

#define MAX_OVERLAP_COUNT 256

typedef struct comp_area_box {
	float width;
	float height;
	float offsetX;
	float offsetY;
	entity_t overlaps[MAX_OVERLAP_COUNT];
	int overlapCount;
} comp_area_box_t;

typedef struct comp_col_box {
	float width;
	float height;
	float offsetX;
	float offsetY;
} comp_col_box_t;

typedef struct comp_draw_sprite {
	float w;
	float h;
	Texture2D texture;
	float offsetX;
	float offsetY;
	bool visible;
	Color color;
} comp_draw_sprite_t;

typedef struct comp_draw_box {
	float width;
	float height;
	float offsetX;
	float offsetY;
	bool visible;
	Color color;
} comp_draw_box_t;

typedef struct comp_draw_circle {
	float radius;
	float offsetX;
	float offsetY;
	bool visible;
	Color color;
} comp_draw_circle_t;

#define MAX_TEXT_LENGTH 256 //probably a bit small

typedef struct comp_draw_text {
	char text[MAX_TEXT_LENGTH];
	int size;
	float offsetX;
	float offsetY;
	Color color;
	int priority;
	bool visible;
} comp_draw_text_t;

typedef struct comp_ui_button {
	entity_t text;
	float offsetX;
	float offsetY;
	Color color;
	int priority;
	bool visible;
	bool pressed;
} comp_ui_button_t;

typedef enum component_type {
	COMP_POSITION	 = 1 << 0,
	COMP_ROTATION	 = 1 << 1,
	COMP_VELOCITY	 = 1 << 2,
	COMP_INPUT		 = 1 << 3,
	COMP_AREA_BOX	 = 1 << 4,
	COMP_COL_BOX	 = 1 << 5,
	COMP_DRAW_SPRITE = 1 << 6,
	COMP_DRAW_BOX	 = 1 << 7,
	COMP_DRAW_CIRCLE = 1 << 8,
	COMP_DRAW_TEXT	 = 1 << 9,
} component_type_t;

typedef struct components {
	comp_position_t position[MAX_ENTITIES];
	comp_rotation_t rotation[MAX_ENTITIES];
	comp_velocity_t velocity[MAX_ENTITIES];
	comp_input_t input[MAX_ENTITIES];
	comp_area_box_t area_box[MAX_ENTITIES];
	comp_col_box_t col_box[MAX_ENTITIES];
	comp_draw_sprite_t draw_sprite[MAX_ENTITIES];
	comp_draw_box_t draw_box[MAX_ENTITIES];
	comp_draw_circle_t draw_circle[MAX_ENTITIES];
	comp_draw_text_t draw_text[MAX_ENTITIES];
	component_type_t c_mask[MAX_ENTITIES];
} components_t;

void component_add(components_t* components,
				   entity_t entity,
				   component_type_t component);
void component_remove(components_t* components,
					  entity_t entity,
					  component_type_t component);
