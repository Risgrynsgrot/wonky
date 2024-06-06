#include "components.h"
#include "rendersystem.h"
#include "scriptloader.h"
#include <assert.h>
#include <lauxlib.h>
#include <lua.h>
#include <stdio.h>
#include <string.h>

ecs_id_t id_comp_position;
ecs_id_t id_comp_rotation;
ecs_id_t id_comp_velocity;
ecs_id_t id_comp_input;
ecs_id_t id_comp_area_box;
ecs_id_t id_comp_col_box;
ecs_id_t id_comp_draw_sprite;
ecs_id_t id_comp_draw_box;
ecs_id_t id_comp_draw_circle;
ecs_component_string_t ecs_component_strings[COMPONENT_COUNT];
int ecs_component_string_count;

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

ecs_id_t ecs_string_to_componentid(const char* value) {
	for(int i = 0; i < ecs_component_string_count; i++) {
		if(strcmp(value, ecs_component_strings[i].name) != 0) {
			continue;
		}
		ecs_id_t result = ecs_component_strings[i].id;
		return result;
	}
	assert(false && "tried to get nonexistent component, did you misspell?");
	return 0;
}

//int ecs_lua_add_position(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs				  = script_get_userdata(L, "ecs");
//	comp_position_t* position = ecs_add(ecs, entity, id_comp_position, NULL);
//	lua_table_get(L, "value");
//	position->value.x = table_get_number(L, "x");
//	position->value.y = table_get_number(L, "y");
//	lua_pop(L, 1);
//	return 1;
//}
//
//int ecs_lua_add_rotation(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs				  = script_get_userdata(L, "ecs");
//	comp_rotation_t* rotation = ecs_add(ecs, entity, id_comp_velocity, NULL);
//	rotation->angle			  = table_get_number(L, "angle");
//	lua_pop(L, 1);
//	return 1;
//}
//
//int ecs_lua_add_velocity(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs				  = script_get_userdata(L, "ecs");
//	comp_velocity_t* velocity = ecs_add(ecs, entity, id_comp_velocity, NULL);
//	lua_table_get(L, "value");
//	velocity->value.x = table_get_number(L, "x");
//	velocity->value.y = table_get_number(L, "y");
//	lua_pop(L, 1);
//	return 1;
//}
//
//int ecs_lua_add_input(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs			= script_get_userdata(L, "ecs");
//	comp_input_t* input = ecs_add(ecs, entity, id_comp_input, NULL);
//	input->input_id		= table_get_int(L, "input_id");
//	lua_table_get(L, "direction");
//	input->direction.x = table_get_number(L, "x");
//	input->direction.y = table_get_number(L, "y");
//	lua_pop(L, 1);
//	input->interact		  = table_get_bool(L, "interact");
//	input->open_inventory = table_get_bool(L, "open_inventory");
//	return 1;
//}
//
//int ecs_lua_add_area_box(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs				  = script_get_userdata(L, "ecs");
//	comp_area_box_t* area_box = ecs_add(ecs, entity, id_comp_area_box, NULL);
//	area_box->width			  = table_get_number(L, "width");
//	area_box->height		  = table_get_number(L, "height");
//	area_box->offset_x		  = table_get_number(L, "offset_x");
//	area_box->offset_y		  = table_get_number(L, "offset_y");
//	area_box->overlapCount	  = 0;
//	return 1;
//}
//
//int ecs_lua_add_col_box(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs				= script_get_userdata(L, "ecs");
//	comp_col_box_t* col_box = ecs_add(ecs, entity, id_comp_col_box, NULL);
//	col_box->width			= table_get_number(L, "width");
//	col_box->height			= table_get_number(L, "height");
//	col_box->offset_x		= table_get_number(L, "offset_x");
//	col_box->offset_y		= table_get_number(L, "offset_y");
//	return 1;
//}
//
//int ecs_lua_add_draw_sprite(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs = script_get_userdata(L, "ecs");
//	ecs_add(ecs, entity, id_comp_draw_sprite, NULL);
//	const char* path = table_get_string(L, "path");
//	render_load_sprite(ecs, path, entity);
//	return 1;
//}
//
//int ecs_lua_add_draw_box(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs				  = script_get_userdata(L, "ecs");
//	comp_draw_box_t* draw_box = ecs_add(ecs, entity, id_comp_draw_box, NULL);
//	draw_box->width			  = table_get_number(L, "width");
//	draw_box->height		  = table_get_number(L, "height");
//	draw_box->offset_x		  = table_get_number(L, "offset_x");
//	draw_box->offset_y		  = table_get_number(L, "offset_y");
//	draw_box->color			  = table_get_color(L, "color");
//	draw_box->visible		  = table_get_bool(L, "visible");
//	return 1;
//}
//
//int ecs_lua_add_draw_circle(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs = script_get_userdata(L, "ecs");
//	comp_draw_circle_t* draw_circle =
//		ecs_add(ecs, entity, id_comp_draw_circle, NULL);
//	draw_circle->radius	  = table_get_number(L, "radius");
//	draw_circle->offset_x = table_get_number(L, "offset_x");
//	draw_circle->offset_y = table_get_number(L, "offset_y");
//	draw_circle->color	  = table_get_color(L, "color");
//	draw_circle->visible  = table_get_bool(L, "visible");
//	return 1;
//}
//
//#define LUA_TRY_ADD_COMP(T)                                                    \
//	if(ecs_string_to_componentid(component) == id_comp_##T) {                  \
//		return ecs_lua_add_##T(L, entity);                                     \
//	}
//
//int ecs_lua_add_component(lua_State* L) {
//	printf("waba?\n");
//	ecs_id_t entity = lua_tointeger(L, -2);
//	if(lua_istable(L, -1)) { //the table with the component info
//		lua_getfield(L, -1, "type");
//		if(lua_isstring(L, -1)) {
//			const char* component = lua_tostring(L, -1);
//			lua_pop(L, 1);
//			printf("component: %s\n", component);
//			LUA_TRY_ADD_COMP(position);
//			LUA_TRY_ADD_COMP(rotation);
//			LUA_TRY_ADD_COMP(velocity);
//			LUA_TRY_ADD_COMP(input);
//			LUA_TRY_ADD_COMP(area_box);
//			LUA_TRY_ADD_COMP(col_box);
//			LUA_TRY_ADD_COMP(draw_sprite);
//			LUA_TRY_ADD_COMP(draw_box);
//			LUA_TRY_ADD_COMP(draw_circle);
//		} else {
//			printf("not a string\n");
//		}
//	} else {
//		printf("not a table\n");
//	}
//	return 0;
//}

void ecs_components_register(ecs_t* ecs) {

	ecs_component_string_count = 0;
	ECS_REGISTER_COMPONENT(ecs, comp_position);
	ECS_REGISTER_COMPONENT(ecs, comp_rotation);
	ECS_REGISTER_COMPONENT(ecs, comp_velocity);
	ECS_REGISTER_COMPONENT(ecs, comp_input);
	ECS_REGISTER_COMPONENT(ecs, comp_area_box);
	ECS_REGISTER_COMPONENT(ecs, comp_col_box);
	ECS_REGISTER_COMPONENT(ecs, comp_draw_sprite);
	ECS_REGISTER_COMPONENT(ecs, comp_draw_box);
	ECS_REGISTER_COMPONENT(ecs, comp_draw_circle);
}

//static const struct luaL_Reg ecs_methods[] = {
	//{"AddComponent", ecs_lua_add_component},
	 //{		  NULL,					NULL}
//};

//void ecs_lua_register_module(lua_State* L) {
	//int pos = lua_gettop(L);

	//luaL_register(L, "ECS", ecs_methods);
	//printf("registered methods\n");
	//luaL_setfuncs(L, ecs_methods, 0);
//}
