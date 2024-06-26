#pragma once
#include <lua.h>
#include <stdbool.h>

typedef struct Vector2 Vector2;

typedef struct network_buffer {
	char buffer[256];
} network_buffer_t;

typedef struct ser_network {
	network_buffer_t net_buf;
} ser_network_t;

typedef struct ser_lua {
	lua_State* L;
} ser_lua_t;

typedef struct serializer {
	union ser_u {
		ser_lua_t lua;
		ser_network_t network;
	} ser;

	bool (*ser_int)(struct serializer* ser, int* value, const char* name);
	bool (*ser_float)(struct serializer* ser, float* value, const char* name);
	bool (*ser_double)(struct serializer* ser, double* value, const char* name);
	bool (*ser_vec2)(struct serializer* ser, Vector2* value, const char* name);
	bool (*ser_bool)(struct serializer* ser, bool* value, const char* name);
	bool (*ser_string)(struct serializer* ser, char* value, const char* name);
} serializer_t;

#define DECL_SER_T(T, NAME)                                                    \
	bool read_##NAME##_network(serializer_t* ser, T* value, const char* name); \
	bool read_##NAME##_lua(serializer_t* ser, T* value, const char* name);

#define DEF_SER_T(T, NAME)                                                     \
	bool read_##NAME##_network(                                                \
		serializer_t* ser, T* value, const char* name) {                       \
		(void)ser;                                                             \
		(void)value;                                                           \
		(void)name;                                                            \
		return true;                                                           \
	}                                                                          \
	bool read_##NAME##_lua(serializer_t* ser, T* value, const char* name) {    \
		*value = table_get_##NAME(ser->ser.lua.L, name);                       \
		return true;                                                           \
	}

bool read_string_network(serializer_t* ser, char* value, const char* name);
bool read_string_lua(serializer_t* ser, char* value, const char* name);

DECL_SER_T(Vector2, vector2)
DECL_SER_T(int, int)
DECL_SER_T(float, float)
DECL_SER_T(double, double)
DECL_SER_T(bool, bool)

serializer_t new_reader_lua(ser_lua_t ser_lua);
serializer_t new_reader_network(ser_network_t ser_network);

typedef struct comp_position comp_position_t;
void ser_position(serializer_t* ser, comp_position_t* position);
