#pragma once
#include "pico_ecs.h"
#include "serializer.h"
#include <lua.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

typedef struct ecs_component_string {
	char name[64];
	ecs_id_t id;
} ecs_component_string_t;

#define COMPONENT_COUNT                                                        \
	64 //This should be a constant in pico ecs, can't get it to work now though
extern ecs_component_string_t ecs_component_strings[COMPONENT_COUNT];

#define ECS_COMPONENT_T(T)                                                     \
	extern ecs_id_t id_##T;                                                    \
	typedef struct T

#define ECS_COMPONENTS_TYPE_ITER(_F, ...)                                      \
	_F(position, POSITION, 0, NULL, NULL, __VA_ARGS__)                         \
	_F(rotation, ROTATION, 1, NULL, NULL, __VA_ARGS__)                         \
	_F(velocity, VELOCITY, 2, NULL, NULL, __VA_ARGS__)                         \
	_F(input, INPUT, 3, NULL, NULL, __VA_ARGS__)                               \
	_F(area_box, AREA_BOX, 4, NULL, NULL, __VA_ARGS__)                         \
	_F(col_box, COL_BOX, 5, NULL, NULL, __VA_ARGS__)                           \
	_F(draw_sprite, DRAW_SPRITE, 6, NULL, NULL, __VA_ARGS__)                   \
	_F(draw_box, DRAW_BOX, 7, NULL, NULL, __VA_ARGS__)                         \
	_F(draw_circle, DRAW_CIRCLE, 8, NULL, NULL, __VA_ARGS__)\
	_F(mover, MOVER, 9, NULL, NULL, __VA_ARGS__)

#define DECL_ENUM_COMPONENTS(lc, uc, i, ...) COMPONENT_##uc = i,

#define REGISTER_COMPONENTS(lc, uc, i, constructor, destructor, ECS, ...)      \
	id_comp_##lc = ecs_register_component(                                     \
		ECS, sizeof(comp_##lc##_t), constructor, destructor);                  \
	ecs_component_register_string(                                             \
		(ecs_component_string_t){.name = #lc, .id = id_comp_##lc});            \
	ecs_component_string_count++;

#define DECL_COMPONENT_FIELD(T, NAME) T NAME;
#define DECL_COMPONENT_IDS(lc, uc, i, ...) ecs_id_t id_comp_##lc;

#define DECL_COMPONENT_STRUCT(T, ITER)                                         \
	typedef struct comp_##T {                                                  \
		ITER(DECL_COMPONENT_FIELD, void)                                       \
	} comp_##T##_t;

typedef enum component_types {
	ECS_COMPONENTS_TYPE_ITER(DECL_ENUM_COMPONENTS, void)
} component_types_e;

ECS_COMPONENT_T(comp_position) {
	Vector2 value;
	Vector2 grid_pos;
	int layer;
}

comp_position_t;
void ser_position(serializer_t* ser, comp_position_t* position);

ECS_COMPONENT_T(comp_rotation) {
	float angle;
}

comp_rotation_t;
void ser_rotation(serializer_t* ser, comp_rotation_t* rotation);

ECS_COMPONENT_T(comp_velocity) {
	Vector2 value;
}

comp_velocity_t;
void ser_velocity(serializer_t* ser, comp_velocity_t* velocity);

ECS_COMPONENT_T(comp_input) {
	int input_id;
	Vector2 direction;
	bool interact;
	bool open_inventory;
}

comp_input_t;
void ser_input(serializer_t* ser, comp_input_t* input);

#define MAX_OVERLAP_COUNT 256

ECS_COMPONENT_T(comp_area_box) {
	float width;
	float height;
	float offset_x;
	float offset_y;
	ecs_id_t overlaps[MAX_OVERLAP_COUNT];
	int overlapCount;
}

comp_area_box_t;
void ser_area_box(serializer_t* ser, comp_area_box_t* area_box);

ECS_COMPONENT_T(comp_col_box) {
	float width;
	float height;
	float offset_x;
	float offset_y;
}

comp_col_box_t;
void ser_col_box(serializer_t* ser, comp_col_box_t* col_box);

ECS_COMPONENT_T(comp_draw_sprite) {
	float width;
	float height;
	Texture2D texture;
	float offset_x;
	float offset_y;
	bool visible;
	Color color;
}

comp_draw_sprite_t;
void ser_draw_sprite(serializer_t* ser, comp_draw_sprite_t* draw_sprite);

ECS_COMPONENT_T(comp_draw_box) {
	float width;
	float height;
	float offset_x;
	float offset_y;
	bool visible;
	Color color;
}

comp_draw_box_t;
void ser_draw_box(serializer_t* ser, comp_draw_box_t* draw_box);

ECS_COMPONENT_T(comp_draw_circle) {
	float radius;
	float offset_x;
	float offset_y;
	bool visible;
	Color color;
}

comp_draw_circle_t;
void ser_draw_circle(serializer_t* ser, comp_draw_circle_t* draw_circle);

ECS_COMPONENT_T(comp_mover) {
	float movement_speed; //movespeed in squares per second
	Vector2 target_tile;
	Vector2 current_tile;
	float _move_cooldown;
}

comp_mover_t;
void ser_mover(serializer_t* ser, comp_mover_t* mover);

void ecs_component_register_string(ecs_component_string_t value);
void* ecs_add_component_string(ecs_t* ecs, ecs_id_t entity, const char* value);
bool ecs_string_to_componentid(ecs_id_t* out_result, const char* value);
void ecs_components_register(ecs_t* ecs);
void ecs_lua_register_module(lua_State* L);

//typedef struct comp_draw_text {
//	char text[MAX_TEXT_LENGTH];
//	int size;
//	float offset_x;
//	float offset_y;
//	Color color;
//	int priority;
//	bool visible;
//} comp_draw_text_t;
//
//typedef struct comp_ui_button {
//	entity_t text;
//	float offset_x;
//	float offset_y;
//	Color color;
//	int priority;
//	bool visible;
//	bool pressed;
//} comp_ui_button_t;
