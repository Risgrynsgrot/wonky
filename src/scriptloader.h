#pragma once
#include "lua.h"
#include <stdbool.h>

lua_State* script_lua_init(void);
void script_lua_close(lua_State* L);
void script_load(lua_State* L, const char* path);

double script_get_number(lua_State* L, const char* value);
int script_get_int(lua_State* L, const char* value);
const char* script_get_string(lua_State* L, const char* value);
void* script_get_userdata(lua_State* L, const char* value);

double lua_table_get_number(lua_State* L, const char* value);
int lua_table_get_int(lua_State* L, const char* value);
const char* lua_table_get_string(lua_State* L, const char* value);

#define lua_table_get(L, value) \
	do { \
		lua_getfield(L, -1, value); \
		if(!lua_istable(L, -1)) { \
			return 0; \
		}\
	} while(0)
