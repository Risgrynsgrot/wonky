#include "components.h"
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
		else {
			printf("%s does not equal %s", value, ecs_component_strings[i].name);
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
		else {
			printf("%s does not equal %s", value, ecs_component_strings[i].name);
		}
	}
	assert(false && "tried to get nonexistent component, did you misspell?");
	return 0;
}

int ecs_lua_add_position(lua_State* L) {
	ecs_t* ecs				  = lua_touserdata(L, -2);
	ecs_id_t entity			  = lua_tointeger(L, -3);
	printf("entityID: %d\n", entity);
	comp_position_t* position = ecs_add(ecs, entity, id_comp_position, NULL);

	lua_getfield(L, -1, "value");
	lua_getfield(L, -1, "x");
	position->value.x = lua_tonumber(L, -1);
	lua_getfield(L, -2, "y");
	position->value.y = lua_tonumber(L, -1);

	printf("Added position component from lua!: %f, %f",
		   position->value.x,
		   position->value.y);
	return 1;
}

int ecs_lua_add_component(lua_State* L) {
	if(lua_istable(L, -1)) {
		lua_getfield(L, -1, "type");
		if(lua_isstring(L, -1)) {
			const char* component = lua_tostring(L, -1);
			lua_pop(L, 2);
			printf("component: %s\n", component);
			if(ecs_string_to_componentid(component) == id_comp_position) {
				return ecs_lua_add_position(L);
			}
		}
		else {
			printf("not a string\n");
		}
	}
	else {
		printf("not a table\n");
	}
	return 0;
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

static const struct luaL_Reg ecs_methods[] = {
	{"AddComponent", ecs_lua_add_component},
	 {		  NULL,					NULL}
};

void ecs_lua_register_module(lua_State* L) {
	//int pos = lua_gettop(L);

	luaL_register(L, "Ecs", ecs_methods);
	printf("registered methods\n");
	//luaL_setfuncs(L, ecs_methods, 0);
}
