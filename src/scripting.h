#pragma once
#include "lua.h"

typedef struct entity entity_t;
typedef struct gameworld gameworld_t;

//d double
//i int 
//u userdata
//l lightuserdata
//s string
//b bool
void call_lua_event(lua_State* L, gameworld_t* world, const char* event_name, entity_t* entity, const char* signature, ...);
void call_lua_event_all(lua_State* L, const char* event_name, const char* signature, ...);

void event_call_on_create(lua_State* L, gameworld_t* world, entity_t* entity);
