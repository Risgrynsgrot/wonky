#pragma once
#include "lua.h"
#include <stdbool.h>

lua_State* script_lua_init(void);
void script_lua_close(lua_State* L);
void script_load(lua_State* L, const char* path);

double script_get_number(lua_State* L, const char* value);
int script_get_int(lua_State* L, const char* value);
const char* script_get_string(lua_State* L, const char* value);
bool script_get_bool(lua_State* L, const char* value);
void* script_get_userdata(lua_State* L, const char* value);

double table_serialize_number(lua_State* L, const char* value);
int table_serialize_int(lua_State* L, const char* value);
const char* table_serialize_string(lua_State* L, const char* value);
bool table_serialize_bool(lua_State* L, const char* value);
void* table_serialize_userdata(lua_State* L, const char* value);

typedef struct Vector2 Vector2;
Vector2 table_serialize_vector2(lua_State* L, const char* value);

typedef struct Color Color;
Color table_serialize_color(lua_State* L, const char* value);

#define lua_table_serialize(L, value)                                                \
	do {                                                                       \
		lua_getfield(L, -1, value);                                            \
		if(!lua_istable(L, -1)) {                                              \
			return 0;                                                          \
		}                                                                      \
	} while(0)

//int lua_get_movement_x(lua_State* L) {
//	ecs_t* ecs				  = script_get_userdata(L, "ecs");
//	ecs_id_t entity			  = lua_tointeger(L, -1);
//	comp_movement_t* movement = ecs_get(ecs, entity, id_comp_movement, NULL);
//	lua_pushnumber(L, movement->x); //add pushvector
//	return 1;
//}

//Somehow the components need a reference to it's entity
//they also need their id so it's easy to switch on the type
//also needs to do things with the metatable to get field like behavior
#define DEF_LUA_FIELD_GETSET(T, LT, FIELD)                                     \
	int lua_get_##T_##FIELD(lua_State* L) {                                    \
		ecs_t* ecs		= script_get_userdata(L, "ecs");                       \
		ecs_id_t entity = lua_tointeger(L, -1);                                \
		T* value		= ecs_get(ecs, entity, id_##T, NULL);                  \
		lua_push##LT(L, value->##FIELD);                                       \
		return 1;                                                              \
	}                                                                          \
	int lua_set_##T_##FIELD(lua_State* L) {                                    \
		ecs_t* ecs		= script_get_userdata(L, "ecs");                       \
		ecs_id_t entity = lua_to##LT(L, -1);                                   \
		T* value		= ecs_get(ecs, entity, id_##T, NULL);                  \
		lua_push##LT(L, value->##FIELD);                                       \
		return 1;                                                              \
	}

