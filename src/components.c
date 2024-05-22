#include "components.h"
#include <assert.h>
#include <lua.h>
#include <string.h>
#include <stdio.h>

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
		if(strcmp(value, ecs_component_strings[i].name) == 0) {
			ecs_id_t result = ecs_component_strings[i].id;
			return result;
		}
	}
	assert(false && "tried to get nonexistent component, did you misspell?");
	return NULL;
}

void ecs_lua_add_position(lua_State* L) {
	ecs_t* ecs				  = lua_touserdata(L, 1);
	ecs_id_t entity			  = lua_tointeger(L, 2);
	comp_position_t* position = ecs_add(ecs, entity, id_comp_position, NULL);

	lua_gettable(L, 3);
	lua_getfield(L, -1, "value");
	lua_getfield(L, -2, "x");
	position->value.x = lua_tonumber(L, -1);
	lua_getfield(L, -3, "y");
	position->value.y = lua_tonumber(L, -1);

	printf("Added position component from lua!: %f, %f",
		   position->value.x,
		   position->value.y);
}

void ecs_lua_add_component(lua_State* L) {
	lua_gettable(L, 3);
	lua_getfield(L, -1, "type");
	const char* component = lua_tostring(L, -1);
	if(ecs_string_to_componentid(component) == id_comp_position) {
		ecs_lua_add_position(L);
	}
}

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
