#include "scriptloader.h"
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>

lua_State* script_lua_init(void) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	return L;
}

void script_lua_close(lua_State* L) {
	lua_close(L);
}

void script_load(lua_State* L, const char* path) {
	int status;
	status = luaL_dofile(L, path);
	if(status) {
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
	}
	return;
}

double script_get_number(lua_State* L, const char* value) {
	lua_getglobal(L, value);

	if(lua_isnumber(L, -1)) {
		double result = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return result;
	}
	printf("LUA TRIED TO GET NUMBER %s BUT FAILED, RETURNING 0\n", value);
	return 0;
}

int script_get_int(lua_State* L, const char* value) {
	lua_getglobal(L, value);

	if(lua_isnumber(L, -1)) {
		int result = lua_tointeger(L, -1);
		lua_pop(L, 1);
		return result;
	}
	printf("LUA TRIED TO GET INTEGER %s BUT FAILED, RETURNING 0\n", value);
	return 0;
}

const char* script_get_string(lua_State* L, const char* value) {
	lua_getglobal(L, value);

	if(lua_isstring(L, -1)) {
		const char* result = lua_tostring(L, -1);
		lua_pop(L, 1);
		return result;
	}
	printf("LUA TRIED TO GET String %s BUT FAILED, RETURNING EMPTY STRING\n", value);
	return "";
}
