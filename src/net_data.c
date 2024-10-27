#include "net_data.h"
#include "gameworld.h"

void ser_net_test(serializer_t* ser, void* data) {
	net_test_t* net_test = data;
	ser->ser_int(ser, &net_test->a, "a");
	ser->ser_bool(ser, &net_test->extra, "extra");
	ser->ser_int(ser, &net_test->b, "b");
	ser->ser_int(ser, &net_test->c, "c");
	ser->ser_int(ser, &net_test->d, "d");
}

void ser_net_move(serializer_t* ser, void* data) {
	net_move_t* net_move = data;
	ser->ser_int(ser, &net_move->entity_id, "entity_id");
	ser->ser_vec2(ser, &net_move->from_tile, "from_tile");
	ser->ser_vec2(ser, &net_move->to_tile, "to_tile");
}

void ser_spawn_entity(serializer_t* ser, void* data) {
	net_spawn_entity_t* net_spawn_entity = data;

	ser->ser_net_string(ser, &net_spawn_entity->entity_type, "entity_type");
	ser->ser_int(ser, &net_spawn_entity->controller, "controller");
	ser->ser_vec2(ser, &net_spawn_entity->position, "position");
}

void handle_spawn_entity(gameworld_t* world, net_spawn_entity_t* data) {
	entity_t entity = entity_new(&world->entities);
	entity_set_type(&world->entities, entity, (char*)data->entity_type.str);
	comp_position_t* pos = &world->entities.position_a[entity.id];
	pos->grid_pos = data->position;
	map_add_entity(&world->map, 0, pos->grid_pos, entity);
}
