#include "net_data.h"

void ser_net_test(serializer_t* ser, void* data) {
	net_test_t* net_test = (net_test_t*)data;
	ser->ser_int(ser, &net_test->a, "a");
	ser->ser_bool(ser, &net_test->extra, "extra");
	ser->ser_int(ser, &net_test->b, "b");
	ser->ser_int(ser, &net_test->c, "c");
	ser->ser_int(ser, &net_test->d, "d");
}

void ser_net_move(serializer_t* ser, void* data) {
	net_move_t* net_move = (net_move_t*)data;
	ser->ser_int(ser, &net_move->entity_id, "entity_id");
	ser->ser_vec2(ser, &net_move->from_tile, "from_tile");
	ser->ser_vec2(ser, &net_move->to_tile, "to_tile");
}
