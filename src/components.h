#pragma once
#include "pico_ecs.h"
#include <raylib.h>
#include <stdbool.h>

#define ECS_COMPONENT_T(T) \
	extern ecs_id_t id_##T;\
	typedef struct T

#define ECS_REGISTER_COMPONENT(ECS, T) \
	id_##T = \
		ecs_register_component(ECS, sizeof(T##_t), NULL, NULL);

ECS_COMPONENT_T(comp_position) {
	Vector2 value;
} comp_position_t;

ECS_COMPONENT_T(comp_rotation) {
	float angle;
} comp_rotation_t;

ECS_COMPONENT_T(comp_velocity) {
	Vector2 value;
} comp_velocity_t;

ECS_COMPONENT_T(comp_input) {
	int input_id;
	Vector2 direction;
	bool interact;
	bool open_inventory;
} comp_input_t;

#define MAX_OVERLAP_COUNT 256

ECS_COMPONENT_T(comp_area_box) {
	float width;
	float height;
	float offsetX;
	float offsetY;
	ecs_id_t overlaps[MAX_OVERLAP_COUNT];
	int overlapCount;
} comp_area_box_t;

ECS_COMPONENT_T(comp_col_box) {
	float width;
	float height;
	float offsetX;
	float offsetY;
} comp_col_box_t;

ECS_COMPONENT_T(comp_draw_sprite) {
	float w;
	float h;
	Texture2D texture;
	float offsetX;
	float offsetY;
	bool visible;
	Color color;
} comp_draw_sprite_t;

ECS_COMPONENT_T(comp_draw_box) {
	float width;
	float height;
	float offsetX;
	float offsetY;
	bool visible;
	Color color;
} comp_draw_box_t;

ECS_COMPONENT_T(comp_draw_circle) {
	float radius;
	float offsetX;
	float offsetY;
	bool visible;
	Color color;
} comp_draw_circle_t;

void ecs_components_register(ecs_t* ecs);

//typedef struct comp_draw_text {
//	char text[MAX_TEXT_LENGTH];
//	int size;
//	float offsetX;
//	float offsetY;
//	Color color;
//	int priority;
//	bool visible;
//} comp_draw_text_t;
//
//typedef struct comp_ui_button {
//	entity_t text;
//	float offsetX;
//	float offsetY;
//	Color color;
//	int priority;
//	bool visible;
//	bool pressed;
//} comp_ui_button_t;
