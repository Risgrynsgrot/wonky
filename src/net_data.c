#include "net_data.h"
#include "gameworld.h"
#include "movesystem.h"
#include "scripting.h"
#include <stdio.h>

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

void net_handle_move(gameworld_t* world,
					 net_move_t* data,
					 net_player_t* player) {
	entity_t entity = player->entity;
	comp_mover_t* mover = &world->entities.mover_a[entity.id];
	comp_position_t* position = &world->entities.position_a[entity.id];
	Vector2 direction = Vector2Subtract(data->to_tile, data->from_tile);
	move_unit(world, entity, position, mover, direction);
}

void ser_spawn_entity(serializer_t* ser, void* data) {
	net_spawn_entity_t* spawn_entity = data;

	ser->ser_net_string(ser, &spawn_entity->entity_type, "entity_type");
	printf("entity type in packet: %s\n",
		   (char*)spawn_entity->entity_type.str);
	ser->ser_int(ser, &spawn_entity->controller, "controller");
	ser->ser_vec2(ser, &spawn_entity->position, "position");
}

void net_handle_spawn_entity(gameworld_t* world, net_spawn_entity_t* data) {
	entity_t entity = entity_new(&world->entities);
	entity_set_type(&world->entities, entity, (char*)data->entity_type.str);
	event_call_on_create(world->L, world, &entity);
	comp_position_t* pos = &world->entities.position_a[entity.id];
	pos->grid_pos		 = data->position;
	map_add_entity(&world->map, 0, pos->grid_pos, entity);
}

void ser_entity_state(serializer_t* ser, void* data) {

	net_entity_state_t* state = data;
	entity_t* entity = &state->entity;
	int components;
	ser->ser_int(ser, &components, "components");
	state->components = components;
	ser->ser_entity(ser, entity, "entity");


	//this somehow. Need to convert from net entity to local
	//put it in a ring buffer for the past
	//handle at the end or beginning of frame I think, do the interpolation
	//there

	for(int i = 0; i < COMPONENT_COUNT; i++) {
		bool has_comp = ((components >> i) & 1) == 1;
		if(has_comp) {
		void* component =
			entity_get_component(&ser->world->entities, *entity, i);
			component_serializer_funcs[i](ser, component);
		}
	}

	//state->data
}
void net_handle_entity_state(gameworld_t* world, net_entity_state_t* data) {
}
