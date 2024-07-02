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

bool read_string_network(serializer_t* ser, char* value, const char* name) {
	(void)ser;
	(void)value;
	(void)name;
	return true;
}

bool read_string_lua(serializer_t* ser, char* value, const char* name) {
	const char* result = table_get_string(ser->ser.lua.L, name);
	strcpy(value, result);
	return true;
}


serializer_t new_reader_lua(ser_lua_t ser_lua) {
	serializer_t result = {
		.ser.lua	= ser_lua,
		.ser_vec2	= read_vector2_lua,
		.ser_int	= read_int_lua,
		.ser_float	= read_float_lua,
		.ser_double = read_double_lua,
		.ser_bool	= read_bool_lua,
		.ser_string = read_string_lua,
	};
	return result;
}

serializer_t new_reader_network(ser_network_t ser_network) {
	serializer_t result = {
		.ser.network = ser_network,
		.ser_vec2	 = read_vector2_network,
		.ser_int	 = read_int_network,
		.ser_float	 = read_float_network,
		.ser_double	 = read_double_network,
		.ser_bool	 = read_bool_network,
		.ser_string	 = read_string_network,
	};
	return result;
}
