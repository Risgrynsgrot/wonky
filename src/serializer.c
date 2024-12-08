#include "serializer.h"
#include "gameworld.h"
#include "network_common.h"
#include "scriptloader.h"
#include <raylib.h>
#include <string.h>

DEF_SER_T(Vector2, vector2)
DEF_SER_T(int32_t, int)
DEF_SER_T(uint32_t, uint)
DEF_SER_T(float, float)
DEF_SER_T(double, double)
DEF_SER_T(bool, bool)
DEF_SER_T(Color, color)
DEF_SER_T(entity_t, entity)

bool read_net_string_network(serializer_t* ser,
							 net_string_t* value,
							 const char* name) {
	net_read_string(&ser->ser.net, value, name);
	return true;
}

bool write_net_string_network(serializer_t* ser,
							  net_string_t* value,
							  const char* name) {
	net_write_string(&ser->ser.net, value, name);
	return true;
}

bool read_string_lua(serializer_t* ser, char* value, const char* name) {
	const char* result = table_get_string(ser->ser.lua.L, name);
	strcpy(value, result);
	return true;
}

bool write_string_lua(serializer_t* ser, char* value, const char* name) {
	table_set_string(ser->ser.lua.L, name, value);
	return true;
}

serializer_t new_reader_lua(ser_lua_t ser_lua, gameworld_t* world) {
	serializer_t result = {
		.ser.lua	= ser_lua,
		.ser_vec2	= read_vector2_lua,
		.ser_int	= read_int_lua,
		.ser_uint	= read_uint_lua,
		.ser_float	= read_float_lua,
		.ser_double = read_double_lua,
		.ser_bool	= read_bool_lua,
		.ser_string = read_string_lua,
		.ser_color	= read_color_lua,
		.ser_entity = read_entity_lua,
		.world		= world,
	};
	return result;
}

serializer_t new_writer_lua(ser_lua_t ser_lua, gameworld_t* world) {
	serializer_t result = {
		.ser.lua	= ser_lua,
		.ser_vec2	= write_vector2_lua,
		.ser_int	= write_int_lua,
		.ser_uint	= write_uint_lua,
		.ser_float	= write_float_lua,
		.ser_double = write_double_lua,
		.ser_bool	= write_bool_lua,
		.ser_string = write_string_lua,
		.ser_color	= write_color_lua,
		.ser_entity = write_entity_lua,
		.world		= world,
	};
	return result;
}

serializer_t new_reader_network(ser_net_t ser_network, gameworld_t* world) {
	serializer_t result = {
		.ser.net		= ser_network,
		.ser_vec2		= read_vector2_network,
		.ser_int		= read_int_network,
		.ser_uint		= read_uint_network,
		.ser_float		= read_float_network,
		.ser_double		= read_double_network,
		.ser_bool		= read_bool_network,
		.ser_color		= read_color_network,
		.ser_net_string = read_net_string_network,
		.ser_entity		= read_entity_network,
		.world			= world,
	};
	return result;
}

serializer_t new_writer_network(ser_net_t ser_network, gameworld_t* world) {
	serializer_t result = {
		.ser.net		= ser_network,
		.ser_vec2		= write_vector2_network,
		.ser_int		= write_int_network,
		.ser_uint		= write_uint_network,
		.ser_float		= write_float_network,
		.ser_double		= write_double_network,
		.ser_bool		= write_bool_network,
		.ser_color		= write_color_network,
		.ser_net_string = write_net_string_network,
		.ser_entity		= write_entity_network,
		.world			= world,
	};
	return result;
}
