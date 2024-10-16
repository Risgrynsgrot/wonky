#pragma once
#include "pico_ecs.h"
#include "serializer.h"
#include <lua.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

//typedef struct ecs_component_string {
//	char name[64];
//	int32_t id;
//} ecs_component_string_t;

#define COMPONENT_COUNT 64
//extern ecs_component_string_t ecs_component_strings[COMPONENT_COUNT];

#define ECS_COMPONENTS_TYPE_ITER(_F, ...)                                      \
	_F(position, POSITION, 0, NULL, NULL, __VA_ARGS__)                         \
	_F(rotation, ROTATION, 1, NULL, NULL, __VA_ARGS__)                         \
	_F(velocity, VELOCITY, 2, NULL, NULL, __VA_ARGS__)                         \
	_F(input, INPUT, 3, NULL, NULL, __VA_ARGS__)                               \
	_F(area_box, AREA_BOX, 4, NULL, NULL, __VA_ARGS__)                         \
	_F(col_box, COL_BOX, 5, NULL, NULL, __VA_ARGS__)                           \
	_F(draw_sprite, DRAW_SPRITE, 6, NULL, NULL, __VA_ARGS__)                   \
	_F(draw_box, DRAW_BOX, 7, NULL, NULL, __VA_ARGS__)                         \
	_F(draw_circle, DRAW_CIRCLE, 8, NULL, NULL, __VA_ARGS__)                   \
	_F(mover, MOVER, 9, NULL, NULL, __VA_ARGS__)                               \
	_F(net_test, NET_TEST, 10, NULL, NULL, __VA_ARGS__)                        \
	_F(net_move, NET_MOVE, 11, NULL, NULL, __VA_ARGS__)

#define DECL_ENUM_COMPONENTS(lc, uc, i, ...) COMPONENT_##uc = i,
#define DECL_BITSET_COMPONENTS(lc, uc, i, ...) COMPONENT_##uc##_BIT = 1 << i,

//#define REGISTER_COMPONENTS(lc, uc, i, constructor, destructor, ECS, ...)      
//	id_comp_##lc = ecs_register_component(                                     
//		ECS, sizeof(comp_##lc##_t), constructor, destructor);                  
//	ecs_component_register_string(                                             
//		(ecs_component_string_t){.name = #lc, .id = id_comp_##lc});            
//	ecs_component_string_count++;

#define DECL_COMPONENT_FIELD(T, NAME) T NAME;
#define DECL_COMPONENT_IDS(lc, uc, i, ...) ecs_id_t id_comp_##lc;

#define DECL_COMPONENT_STRUCT(T, ITER)                                         \
	typedef struct comp_##T {                                                  \
		ITER(DECL_COMPONENT_FIELD, void)                                       \
	} comp_##T##_t;

typedef enum component_types {
	ECS_COMPONENTS_TYPE_ITER(DECL_ENUM_COMPONENTS, void)
} component_types_e;

typedef enum component_bitset {
	ECS_COMPONENTS_TYPE_ITER(DECL_BITSET_COMPONENTS, void)
} component_bitset_e;

typedef struct comp_position {
	Vector2 value;
	Vector2 grid_pos;
	int layer;
}

comp_position_t;

void ser_position(serializer_t* ser, void* data);

typedef struct comp_rotation {
	float angle;
}

comp_rotation_t;

void ser_rotation(serializer_t* ser, void* data);

typedef struct comp_velocity {
	Vector2 value;
}

comp_velocity_t;

void ser_velocity(serializer_t* ser, void* data);

typedef struct comp_input {
	int input_id;
	Vector2 direction;
	bool interact;
	bool open_inventory;
}

comp_input_t;

void ser_input(serializer_t* ser, void* data);

#define MAX_OVERLAP_COUNT 256

typedef struct comp_area_box {
	float width;
	float height;
	float offset_x;
	float offset_y;
	ecs_id_t overlaps[MAX_OVERLAP_COUNT];
	int overlapCount;
}

comp_area_box_t;

void ser_area_box(serializer_t* ser, void* data);

typedef struct comp_col_box {
	float width;
	float height;
	float offset_x;
	float offset_y;
}

comp_col_box_t;

void ser_col_box(serializer_t* ser, void* data);

typedef struct comp_draw_sprite {
	float width;
	float height;
	Texture2D texture;
	float offset_x;
	float offset_y;
	bool visible;
	Color color;
}

comp_draw_sprite_t;

void ser_draw_sprite(serializer_t* ser, void* data);

typedef struct comp_draw_box {
	float width;
	float height;
	float offset_x;
	float offset_y;
	bool visible;
	Color color;
} comp_draw_box_t;

void ser_draw_box(serializer_t* ser, void* data);

typedef struct comp_draw_circle {
	float radius;
	float offset_x;
	float offset_y;
	bool visible;
	Color color;
}

comp_draw_circle_t;

void ser_draw_circle(serializer_t* ser, void* data);

typedef struct comp_mover {
	float movement_speed; //movespeed in squares per second
	float _move_cooldown;
	Vector2 from_tile;
}

comp_mover_t;

void ser_mover(serializer_t* ser, void* data);

typedef struct comp_net_test {
	int32_t a;
	bool extra;
	int32_t b;
	int32_t c;
	int32_t d;
} comp_net_test_t;

void ser_net_test(serializer_t* ser, void* data);

typedef struct comp_net_move { //move this to separate net structs
	Vector2 from_tile;
	Vector2 to_tile;
	int32_t entity_id;
} comp_net_move_t;

void ser_net_move(serializer_t* ser, void* data);

//void ecs_component_register_string(ecs_component_string_t value);
//void* ecs_add_component_string(ecs_t* ecs, ecs_id_t entity, const char* value);
//bool ecs_string_to_componentid(ecs_id_t* out_result, const char* value);
//void ecs_components_register(ecs_t* ecs);
void ecs_lua_register_module(lua_State* L);

typedef struct entities entities_t;
typedef struct entity entity_t;
void* entity_get_component(entities_t* entities,
						   entity_t entity,
						   component_types_e component);

#define DECL_COMPONENT_SERIALIZERS(lc, uc, i, ...) &ser_##lc,

extern void (*component_serializers[])(serializer_t* serializer, void* data);

void lua_register_component_enum(lua_State* L);
#define DECL_COMPONENT_LUA_ENUM(lc, uc, i, ...)                                \
	lua_pushnumber(L, i);                                                      \
	lua_setfield(L, -2, #lc);
