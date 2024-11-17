#include "scriptloader.h"
#include "entity.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <raylib.h>

void script_dumpstack(lua_State* L) {
	int top = lua_gettop(L);
	for(int i = 1; i <= top; i++) {
		printf("%d\t%s\t", i, luaL_typename(L, i));
		switch(lua_type(L, i)) {
		case LUA_TNUMBER:
			printf("%g\n", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			printf("%s\n", lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			printf("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
			break;
		case LUA_TNIL:
			printf("%s\n", "nil");
			break;
		default:
			printf("%p\n", lua_topointer(L, i));
			break;
		}
	}
}

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
	printf("running script: %s\n", path);
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

int32_t script_get_int(lua_State* L, const char* value) {
	lua_getglobal(L, value);

	if(!lua_isnumber(L, -1)) {
		printf("LUA TRIED TO GET INTEGER %s BUT FAILED, RETURNING 0\n", value);
		return 0;
	}
	int32_t result = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

uint32_t script_get_uint(lua_State* L, const char* value) {
	return script_get_number(L, value);
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

float table_get_float(lua_State* L, const char* value) {
	return table_get_number(L, value);
}

double table_get_double(lua_State* L, const char* value) {
	return table_get_number(L, value);
}

int32_t table_get_int(lua_State* L, const char* value) {
	lua_getfield(L, -1, value);
	if(!lua_isnumber(L, -1)) {
		printf("LUA TRIED TO GET INTEGER %s BUT FAILED, RETURNING 0\n", value);
		lua_pop(L, 1);
		return 0;
	}
	int32_t result = lua_tointeger(L, -1);
	lua_pop(L, 1);
	return result;
}

uint32_t table_get_uint(lua_State* L, const char* value) {
	return table_get_number(L, value);
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
		return 0;
	}
	bool result = lua_toboolean(L, -1);
	lua_pop(L, 1);
	return result;
}

Vector2 table_get_vector2(lua_State* L, const char* value) {
	Vector2 result = {0};

	lua_getfield(L, -1, value);
	if(!lua_istable(L, -1)) {
		printf(
			"LUA TRIED TO GET Vector2 %s BUT FAILED, RETURNING EMPTY\n",
			value);
		lua_pop(L, 1);
		return result;
	}
	result.x = table_get_number(L, "x");
	result.y = table_get_number(L, "y");
	printf("Vector2 from lua: %f, %f\n", result.x, result.y);
	lua_pop(L, 1);
	return result;
}

Color table_get_color(lua_State* L, const char* value) {
	Color result = {0};

	lua_getfield(L, -1, value);
	if(!lua_istable(L, -1)) {
		printf("LUA TRIED TO GET Color %s BUT FAILED, RETURNING EMPTY STRING\n",
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

entity_t table_get_entity(lua_State* L, const char* value) {
	entity_t result = {0};

	lua_getfield(L, -1, value);
	if(!lua_istable(L, -1)) {
		printf("LUA TRIED TO GET entity %s BUT FAILED, RETURNING EMPTY\n",
			   value);
		lua_pop(L, 1);
		return result;
	}
	result.id = table_get_number(L, "id");
	result.generation = table_get_number(L, "generation");
	lua_pop(L, 1);
	return result;
}

void table_set_number(lua_State* L, const char* value, double data) {
	lua_pushnumber(L, data);
	lua_setfield(L, -2, value);
}

void table_set_int(lua_State* L, const char* value, int32_t data) {
	lua_pushnumber(L, data);
	lua_setfield(L, -2, value);
}

void table_set_uint(lua_State* L, const char* value, uint32_t data) {
	lua_pushnumber(L, data);
	lua_setfield(L, -2, value);
}

void table_set_float(lua_State* L, const char* value, float data) {
	lua_pushnumber(L, data);
	lua_setfield(L, -2, value);
}

void table_set_double(lua_State* L, const char* value, double data) {
	lua_pushnumber(L, data);
	lua_setfield(L, -2, value);
}

void table_set_string(lua_State* L, const char* value, const char* data) {
	lua_pushstring(L, data);
	lua_setfield(L, -2, value);
}

void table_set_bool(lua_State* L, const char* value, bool data) {
	lua_pushboolean(L, data);
	lua_setfield(L, -2, value);
}

void table_set_userdata(lua_State* L,
						const char* value,
						void* data) { //might be wrong using light user data
	lua_pushlightuserdata(L, data);
	lua_setfield(L, -2, value);
}

void table_set_vector2(lua_State* L, const char* value, Vector2 data) {
	lua_newtable(L);
	lua_setfield(L, -2, value);
	lua_getfield(L, -1, value);
	table_set_number(L, "x", data.x);
	table_set_number(L, "y", data.y);
	lua_pop(L, 1);
	//printf("stack during ser: \n");
	//script_dumpstack(L);
}

void table_set_color(lua_State* L, const char* value, Color data) {
	lua_newtable(L);
	lua_setfield(L, -2, value);
	lua_getfield(L, -1, value);
	table_set_number(L, "r", data.r);
	table_set_number(L, "g", data.g);
	table_set_number(L, "b", data.b);
	table_set_number(L, "a", data.a);
	lua_pop(L, 1);
	//printf("stack during ser: \n");
	//script_dumpstack(L);
}

void table_set_entity(lua_State* L, const char* value, entity_t data) {
	lua_newtable(L);
	lua_setfield(L, -2, value);
	lua_getfield(L, -1, value);
	table_set_number(L, "id", data.id);
	table_set_number(L, "generation", data.generation);
	lua_pop(L, 1);
}
