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

//void ecs_lua_add_position(lua_State* L, ecs_id_t entity) {
//	ecs_t* ecs				  = script_get_userdata(L, "ecs");
//	comp_position_t* position = ecs_add(ecs, entity, id_comp_position, NULL);
//	ser_lua_t ser_lua		  = {.L = L};
//	serializer_t ser		  = new_reader_lua(ser_lua);
//	ser_position(&ser, position);
//}

#define LUA_ADD_COMP(lc, uc, i, ...)                                           \
	int ecs_lua_add_##lc(serializer_t* ser, ecs_id_t entity) {                    \
		ecs_t* ecs		  = script_get_userdata(ser->ser.lua.L, "ecs");                \
		comp_##lc##_t* lc = ecs_add(ecs, entity, id_comp_##lc, NULL);          \
		ser_##lc(ser, lc);                                                    \
		return 0;                                                              \
	}

ECS_COMPONENTS_TYPE_ITER(LUA_ADD_COMP, void)

#define LUA_TRY_ADD_COMP(lc, uc, i, ...)                                       \
	if(ecs_string_to_componentid(component) == id_comp_##lc) {                 \
		return ecs_lua_add_##lc(&ser, entity);                                 \
	}

int ecs_lua_add_component(lua_State* L) {
	printf("waba?\n");
	ecs_id_t entity = lua_tointeger(L, -2);
	if(lua_istable(L, -1)) { //the table with the component info
		lua_getfield(L, -1, "type");
		if(lua_isstring(L, -1)) {
			const char* component = lua_tostring(L, -1);
			lua_pop(L, 1);
			printf("component: %s\n", component);
			serializer_t ser = new_reader_lua((ser_lua_t){.L = L});
			ECS_COMPONENTS_TYPE_ITER(LUA_TRY_ADD_COMP, void)
		} else {
			printf("not a string\n");
		}
	} else {
		printf("not a table\n");
	}
	return 0;
}

void ecs_components_register(ecs_t* ecs) {
	ecs_component_string_count = 0;
	ECS_COMPONENTS_TYPE_ITER(REGISTER_COMPONENTS, ecs, void);
}

static const struct luaL_Reg ecs_methods[] = {
	//{"AddComponent", ecs_lua_add_component},
	{NULL, NULL}
};

void ecs_lua_register_module(lua_State* L) {
	luaL_register(L, "ECS", ecs_methods);
	printf("registered methods\n");
	luaL_setfuncs(L, ecs_methods, 0);
}
