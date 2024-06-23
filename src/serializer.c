#include "serializer.h"
#include "components.h"
#include "scriptloader.h"
#include <raylib.h>
#include <string.h>

DEF_SER_T(Vector2, vector2)
DEF_SER_T(int, int)
DEF_SER_T(float, float)
DEF_SER_T(double, double)
DEF_SER_T(bool, bool)

void ser_string_network(char* value, ser_network_t* ser) {
	(void)ser;
	(void)value;
}

void ser_string_lua(char* value, const char* lua_name, ser_lua_t* ser) {
	if(ser->is_reading) {
		const char* result = table_get_string(ser->L, lua_name);
		strcpy(value, result);
	}
}

void ser_string(char* value, const char* lua_name, serializer_t* serializer) {
	union ser_u* ser = &serializer->ser;
	switch(serializer->mode) {
	case E_NETWORK:
		ser_string_network(value, &ser->network);
		break;
	case E_LUA:
		ser_string_lua(value, lua_name, &ser->lua);
		break;
	}
}

void ser_position(comp_position_t* position, serializer_t* ser) {
	ser_vector2(&position->value, "value", ser);
}
