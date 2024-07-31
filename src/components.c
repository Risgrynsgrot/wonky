#include "components.h"
#include "rendersystem.h"
#include "scriptloader.h"
#include "serializer.h"
#include <assert.h>
#include <lauxlib.h>
#include <lua.h>
#include <stdio.h>
#include <string.h>

ECS_COMPONENTS_TYPE_ITER(DECL_COMPONENT_IDS, void)
ecs_component_string_t ecs_component_strings[COMPONENT_COUNT];
int ecs_component_string_count;

//TODO(risgrynsgrot): Add ecs_lua_add_component_functions that maps component id
//to lua serialize function

void ser_position(serializer_t* ser, comp_position_t* position) {
	ser->ser_vec2(ser, &position->value, "value");
}

void ser_rotation(serializer_t* ser, comp_rotation_t* rotation) {
	ser->ser_float(ser, &rotation->angle, "angle");
}

void ser_velocity(serializer_t* ser, comp_velocity_t* velocity) {
	ser->ser_vec2(ser, &velocity->value, "value");
}

void ser_input(serializer_t* ser, comp_input_t* input) {
	ser->ser_int(ser, &input->input_id, "input_id");
	ser->ser_vec2(ser, &input->direction, "direction");
	ser->ser_bool(ser, &input->interact, "interact");
	ser->ser_bool(ser, &input->open_inventory, "open_directory");
}

void ser_area_box(serializer_t* ser, comp_area_box_t* area_box) {
	ser->ser_float(ser, &area_box->width, "width");
	ser->ser_float(ser, &area_box->height, "height");
	ser->ser_float(ser, &area_box->offset_x, "offset_x");
	ser->ser_float(ser, &area_box->offset_y, "offset_y");
	//ecs_id_t overlaps[MAX_OVERLAP_COUNT];
	//int overlapCount;
}

void ser_col_box(serializer_t* ser, comp_col_box_t* col_box) {
	ser->ser_float(ser, &col_box->width, "width");
	ser->ser_float(ser, &col_box->height, "height");
	ser->ser_float(ser, &col_box->offset_x, "offset_x");
	ser->ser_float(ser, &col_box->offset_y, "offset_y");
}

void ser_draw_sprite(serializer_t* ser, comp_draw_sprite_t* draw_sprite) {
	ser->ser_float(ser, &draw_sprite->width, "width");
	ser->ser_float(ser, &draw_sprite->height, "height");
	ser->ser_float(ser, &draw_sprite->offset_x, "offset_x");
	ser->ser_float(ser, &draw_sprite->offset_y, "offset_y");
	ser->ser_bool(ser, &draw_sprite->visible, "visible");
	ser->ser_color(ser, &draw_sprite->color, "color");
	//ser->ser_Texture2D texture;
}

void ser_draw_box(serializer_t* ser, comp_draw_box_t* draw_box) {
	ser->ser_float(ser, &draw_box->width, "width");
	ser->ser_float(ser, &draw_box->height, "height");
	ser->ser_float(ser, &draw_box->offset_x, "offset_x");
	ser->ser_float(ser, &draw_box->offset_y, "offset_y");
	ser->ser_bool(ser, &draw_box->visible, "visible");
	ser->ser_color(ser, &draw_box->color, "color");
}

void ser_draw_circle(serializer_t* ser, comp_draw_circle_t* draw_circle) {
	ser->ser_float(ser, &draw_circle->radius, "radius");
	ser->ser_float(ser, &draw_circle->offset_x, "offset_x");
	ser->ser_float(ser, &draw_circle->offset_y, "offset_y");
	ser->ser_bool(ser, &draw_circle->visible, "visible");
	ser->ser_color(ser, &draw_circle->color, "color");
}

void ecs_component_register_string(ecs_component_string_t value) {
	int i						 = ecs_component_string_count;
	ecs_component_string_t* dest = &ecs_component_strings[i];
	dest->id					 = value.id;
	strcpy(dest->name, value.name);
}

void* ecs_add_component_string(ecs_t* ecs, ecs_id_t entity, const char* value) {
	for(int i = 0; i < ecs_component_string_count; i++) {
		if(strcmp(value, ecs_component_strings[i].name) == 0) {
			void* result =
				ecs_add(ecs, entity, ecs_component_strings[i].id, NULL);
			return result;
		}
	}
	assert(false && "tried to add nonexistent component, did you misspell?");
	return NULL;
}

bool ecs_string_to_componentid(ecs_id_t* out_result, const char* value) {
	for(int i = 0; i < ecs_component_string_count; i++) {
		if(strcmp(value, ecs_component_strings[i].name) != 0) {
			continue;
		}
		*out_result = ecs_component_strings[i].id;
		return true;
	}
	return false;
}

//void ecs_lua_add_position(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs				  = script_get_userdata(L, "ecs");
//	comp_position_t* position = ecs_add(ecs, entity, id_comp_position, NULL);
//	ser_lua_t ser_lua		  = {.L = L};
//	serializer_t ser		  = new_reader_lua(ser_lua);
//	ser_position(&ser, position);
//}

#define LUA_ADD_COMP(lc, uc, i, ...)                                           \
	int ecs_lua_add_##lc(serializer_t* ser, ecs_id_t entity) {                 \
		ecs_t* ecs		  = script_get_userdata(ser->ser.lua.L, "ecs");        \
		comp_##lc##_t* lc = ecs_add(ecs, entity, id_comp_##lc, NULL);          \
		ser_##lc(ser, lc);                                                     \
		return 0;                                                              \
	}

#define LUA_SET_COMP(lc, uc, i, ...)                                           \
	int ecs_lua_set_##lc(serializer_t* ser, ecs_id_t entity) {                 \
		ecs_t* ecs		  = script_get_userdata(ser->ser.lua.L, "ecs");        \
		comp_##lc##_t* lc = ecs_get(ecs, entity, id_comp_##lc);                \
		ser_##lc(ser, lc);                                                     \
		return 0;                                                              \
	}

//TODO(risgrynsgrot) Create table and return it as part of getting comp
#define LUA_GET_COMP(lc, uc, i, ...)                                           \
	int ecs_lua_get_##lc(serializer_t* ser, ecs_id_t entity) {                 \
		ecs_t* ecs		  = script_get_userdata(ser->ser.lua.L, "ecs");        \
		comp_##lc##_t* lc = ecs_get(ecs, entity, id_comp_##lc);                \
		lua_newtable(ser->ser.lua.L);                                          \
		lua_pushstring(ser->ser.lua.L, #lc);                                   \
		lua_setfield(ser->ser.lua.L, -2, "type");                              \
		printf("stack before ser: \n");                                        \
		script_dumpstack(ser->ser.lua.L);                                      \
		ser_##lc(ser, lc);                                                     \
		printf("stack after ser: \n");                                         \
		script_dumpstack(ser->ser.lua.L);                                      \
		return 1;                                                              \
	}

ECS_COMPONENTS_TYPE_ITER(LUA_ADD_COMP, void)
ECS_COMPONENTS_TYPE_ITER(LUA_SET_COMP, void)
ECS_COMPONENTS_TYPE_ITER(LUA_GET_COMP, void)

#define LUA_TRY_ADD_COMP(lc, uc, i, ...)                                       \
	if(ecs_string_to_componentid(&component, type)) {                          \
		if(component == id_comp_##lc) {                                        \
			return ecs_lua_add_##lc(&ser, entity);                             \
		}                                                                      \
	}

#define LUA_TRY_SET_COMP(lc, uc, i, ...)                                       \
	if(ecs_string_to_componentid(&component, type)) {                          \
		if(component == id_comp_##lc) {                                        \
			return ecs_lua_set_##lc(&ser, entity);                             \
		}                                                                      \
	}

#define LUA_TRY_GET_COMP(lc, uc, i, ...)                                       \
	if(ecs_string_to_componentid(&component, type)) {                          \
		if(component == id_comp_##lc) {                                        \
			return ecs_lua_get_##lc(&ser, entity);                             \
		}                                                                      \
	}

void ecs_lua_try_add_asset(lua_State* L, ecs_id_t entity, const char* type) {
	if(strcmp(type, "asset_sprite") == 0) {
		lua_getfield(L, -1, "path");
		const char* path = lua_tostring(L, -1);
		lua_pop(L, 1);
		ecs_t* ecs = script_get_userdata(L, "ecs");
		render_load_sprite(ecs, path, entity);
	}
}

int ecs_lua_add_component(lua_State* L) {
	printf("waba?\n");
	ecs_id_t entity = lua_tointeger(L, -2);
	if(lua_istable(L, -1)) { //the table with the component info
		lua_getfield(L, -1, "type");
		if(lua_isstring(L, -1)) {
			const char* type = lua_tostring(L, -1);
			lua_pop(L, 1);
			printf("component_type: %s\n", type);
			serializer_t ser = new_reader_lua((ser_lua_t){.L = L});
			ecs_id_t component;
			ECS_COMPONENTS_TYPE_ITER(LUA_TRY_ADD_COMP, void)
			ecs_lua_try_add_asset(L, entity, type);
		} else {
			printf("not a string\n");
		}
	} else {
		printf("not a table\n");
	}
	return 0;
}

int ecs_lua_set_component(lua_State* L) {
	printf("setting component...\n");
	ecs_id_t entity = lua_tointeger(L, -2);
	if(lua_istable(L, -1)) { //the table with the component info
		lua_getfield(L, -1, "type");
		if(lua_isstring(L, -1)) {
			const char* type = lua_tostring(L, -1);
			lua_pop(L, 1);
			printf("component_type: %s\n", type);
			serializer_t ser = new_reader_lua(
				(ser_lua_t){.L = L}); //this might be better to put in userdata
			ecs_id_t component;
			ECS_COMPONENTS_TYPE_ITER(LUA_TRY_SET_COMP, void)
		} else {
			printf("not a string\n");
		}
	} else {
		printf("not a table\n");
	}
	return 0;
}

//TODO(risgrynsgrot) Change type getting to use ints, to reduce strcmp
int ecs_lua_get_component(lua_State* L) {
	ecs_id_t entity = lua_tointeger(L, -2);
	//lua_getfield(L, -1, "type");
	if(lua_isstring(L, -1)) {
		const char* type = lua_tostring(L, -1);
		serializer_t ser = new_writer_lua((ser_lua_t){.L = L});
		ecs_id_t component;
		ECS_COMPONENTS_TYPE_ITER(LUA_TRY_GET_COMP, void)
	} else {
		printf("not a string\n");
	}

	return 1;
}

void ecs_components_register(ecs_t* ecs) {
	ecs_component_string_count = 0;
	ECS_COMPONENTS_TYPE_ITER(REGISTER_COMPONENTS, ecs, void);
}

static const struct luaL_Reg ecs_methods[] = {
	{"add_component", ecs_lua_add_component},
	{"get_component", ecs_lua_get_component},
	{"set_component", ecs_lua_set_component},
	{		   NULL,				  NULL}
};

void ecs_lua_register_module(lua_State* L) {
	luaL_register(L, "ECS", ecs_methods);
	printf("registered methods\n");
	luaL_setfuncs(L, ecs_methods, 0);
}
