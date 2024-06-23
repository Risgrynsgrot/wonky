#pragma once
#include <stdbool.h>
#include <lua.h>

typedef enum ser_mode { E_NETWORK, E_LUA } ser_mode_e;

typedef struct network_buffer {
	char buffer[256];
} network_buffer_t;

typedef struct ser_network {
	network_buffer_t net_buf;
	bool is_reading;
} ser_network_t;

typedef struct ser_lua {
	lua_State* L;
	bool is_reading;
} ser_lua_t;

typedef struct serializer {
	union ser_u {
		ser_lua_t lua;
		ser_network_t network;
	} ser;

	ser_mode_e mode;
} serializer_t;

#define DECL_SER_T(T, NAME)                                                    \
	void ser_##NAME##_network(T* value, ser_network_t* ser);                   \
	void ser_##NAME##_lua(T* value, const char* lua_name, ser_lua_t* ser);                       \
	void ser_##NAME(T* value, const char* lua_name, serializer_t* serializer)

#define DEF_SER_T(T, NAME)                                                     \
	void ser_##NAME##_network(T* value, ser_network_t* ser) {                  \
		(void)ser;                                                             \
		(void)value;                                                           \
	}                                                                          \
	void ser_##NAME##_lua(T* value, const char* lua_name, ser_lua_t* ser) {    \
		if(ser->is_reading) {                                                  \
			*value = table_get_##NAME(ser->L, lua_name);                       \
		}                                                                      \
	}                                                                          \
	void ser_##NAME(                                                           \
		T* value, const char* lua_name, serializer_t* serializer) {            \
		union ser_u* ser = &serializer->ser;                                   \
		switch(serializer->mode) {                                             \
		case E_NETWORK:                                                        \
			ser_##NAME##_network(value, &ser->network);                        \
			break;                                                             \
		case E_LUA:                                                            \
			ser_##NAME##_lua(value, lua_name, &ser->lua);                      \
			break;                                                             \
		}                                                                      \
	}


void ser_string_network(char* value, ser_network_t* ser);
void ser_string_lua(char* value, const char* lua_name, ser_lua_t* ser);
void ser_string(char* value, const char* lua_name, serializer_t* serializer);

typedef struct Vector2 Vector2;
DECL_SER_T(Vector2, vector2);
DECL_SER_T(int, int);
DECL_SER_T(float, float);
DECL_SER_T(double, double);
DECL_SER_T(bool, bool);
