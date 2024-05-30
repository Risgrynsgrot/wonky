#include "scriptloader.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>

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

	if(!lua_isnumber(L, -1)) {
		printf("LUA TRIED TO GET NUMBER %s BUT FAILED, RETURNING 0\n", value);
		return 0;
	}
	double result = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

int script_get_int(lua_State* L, const char* value) {
	lua_getglobal(L, value);

	if(!lua_isnumber(L, -1)) {
		printf("LUA TRIED TO GET INTEGER %s BUT FAILED, RETURNING 0\n", value);
		return 0;
	}
	int result = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

const char* script_get_string(lua_State* L, const char* value) {
	lua_getglobal(L, value);

	if(!lua_isstring(L, -1)) {
		printf(
			"LUA TRIED TO GET STRING %s BUT FAILED, RETURNING EMPTY STRING\n",
			value);
		lua_pop(L, 1);
		return "";
	}
	const char* result = lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}

bool script_get_bool(lua_State* L, const char* value) {
	lua_getglobal(L, value);

	if(!lua_isboolean(L, -1)) {
		printf("LUA TRIED TO GET BOOL %s BUT FAILED, RETURNING EMPTY STRING\n",
			   value);
		lua_pop(L, 1);
		return "";
	}
	bool result = lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

void* script_get_userdata(lua_State* L, const char* value) {
	lua_getglobal(L, value);

	if(!lua_isuserdata(L, -1)) {
		printf(
			"LUA TRIED TO GET USERDATA %s BUT FAILED, RETURNING EMPTY STRING\n",
			value);
		lua_pop(L, 1);
		return NULL;
	}
	void* result = lua_touserdata(L, -1);
	lua_pop(L, 1);
	return result;
}

double table_get_number(lua_State* L, const char* value) {
	lua_getfield(L, -1, value);
	if(!lua_isnumber(L, -1)) {
		printf("LUA TRIED TO GET NUMBER %s BUT FAILED, RETURNING 0\n", value);
		lua_pop(L, 1);
		return 0;
	}
	double result = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return result;
}

int table_get_int(lua_State* L, const char* value) {
	lua_getfield(L, -1, value);
	if(!lua_isnumber(L, -1)) {
		printf("LUA TRIED TO GET INTEGER %s BUT FAILED, RETURNING 0\n", value);
		lua_pop(L, 1);
		return 0;
	}
	int result = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

const char* table_get_string(lua_State* L, const char* value) {
	lua_getfield(L, -1, value);
	if(!lua_isstring(L, -1)) {
		printf(
			"LUA TRIED TO GET STRING %s BUT FAILED, RETURNING EMPTY STRING\n",
			value);
		lua_pop(L, 1);
		return "";
	}
	const char* result = lua_tostring(L, -1);
	lua_pop(L, 1);
	return result;
}

bool table_get_bool(lua_State* L, const char* value) {
	lua_getfield(L, -1, value);
	if(!lua_isboolean(L, -1)) {
		printf("LUA TRIED TO GET BOOL %s BUT FAILED, RETURNING 0\n", value);
		lua_pop(L, 1);
		return false;
	}
	bool result = lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

void* table_get_userdata(lua_State* L, const char* value) {
	lua_getfield(L, -1, value);
	if(!lua_isuserdata(L, -1)) {
		printf("LUA TRIED TO GET USERDATA %s BUT FAILED, RETURNING 0\n", value);
		lua_pop(L, 1);
		return false;
	}
	void* result = lua_touserdata(L, -1);
	lua_pop(L, 1);
	return result;
}

Vector2 table_get_vector2(lua_State* L, const char* value) {
	Vector2 result = {0};

	lua_getfield(L, -1, value);
	if(!lua_istable(L, -1)) {
		printf(
			"LUA TRIED TO GET Vector2 %s BUT FAILED, RETURNING EMPTY STRING\n",
			value);
		lua_pop(L, 1);
		return result;
	}
	result.x = table_get_number(L, "x");
	result.y = table_get_number(L, "y");
	lua_pop(L, 1);
	return result;
}

Color table_get_color(lua_State* L, const char* value) {
	Color result = {0};

	lua_getfield(L, -1, value);
	if(!lua_istable(L, -1)) {
		printf(
			"LUA TRIED TO GET Color %s BUT FAILED, RETURNING EMPTY STRING\n",
			value);
		lua_pop(L, 1);
		return result;
	}
	result.r = table_get_number(L, "r");
	result.g = table_get_number(L, "g");
	result.b = table_get_number(L, "b");
	result.a = table_get_number(L, "a");
	lua_pop(L, 1);
	return result;
}
