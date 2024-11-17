#pragma once
#include "lua.h"
#include <stdbool.h>
#include <stdint.h>

void script_dumpstack (lua_State *L);

lua_State* script_lua_init(void);
void script_lua_close(lua_State* L);
void script_load(lua_State* L, const char* path);

double script_get_number(lua_State* L, const char* value);
int32_t script_get_int(lua_State* L, const char* value);
uint32_t script_get_uint(lua_State* L, const char* value);
const char* script_get_string(lua_State* L, const char* value);
bool script_get_bool(lua_State* L, const char* value);
void* script_get_userdata(lua_State* L, const char* value);

double table_get_number(lua_State* L, const char* value);
int32_t table_get_int(lua_State* L, const char* value);
uint32_t table_get_uint(lua_State* L, const char* value);
float table_get_float(lua_State* L, const char* value);
double table_get_double(lua_State* L, const char* value);
const char* table_get_string(lua_State* L, const char* value);
bool table_get_bool(lua_State* L, const char* value);
void* table_get_userdata(lua_State* L, const char* value);

typedef struct Vector2 Vector2;
Vector2 table_get_vector2(lua_State* L, const char* value);

typedef struct Color Color;
Color table_get_color(lua_State* L, const char* value);

typedef struct entity entity_t;
entity_t table_get_entity(lua_State* L, const char* value);

void table_set_number(lua_State* L, const char* name, double data);
void table_set_int(lua_State* L, const char* name, int32_t data);
void table_set_uint(lua_State* L, const char* name, uint32_t data);
void table_set_float(lua_State* L, const char* name, float data);
void table_set_double(lua_State* L, const char* name, double data);
void table_set_string(lua_State* L, const char* name,  const char* data);
void table_set_bool(lua_State* L, const char* name, bool data);
void table_set_userdata(lua_State* L, const char* name, void* data);

void table_set_vector2(lua_State* L, const char* value, Vector2 data);
void table_set_color(lua_State* L, const char* value, Color data);
void table_set_entity(lua_State* L, const char* value, entity_t data);
