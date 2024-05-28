#pragma once
#include "lua.h"

lua_State* script_lua_init(void);
void script_lua_close(lua_State* L);
void script_load(lua_State* L, const char* path);

double script_get_number(lua_State* L, const char* value);
int script_get_int(lua_State* L, const char* value);
const char* script_get_string(lua_State* L, const char* value);
