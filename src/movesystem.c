#include "movesystem.h"
#include "components.h"
#include "network_common.h"
#include "raymath.h"
#include <stdio.h>

void ecs_register_move_systems(ecs_t* ecs,
							   map_t* map,
							   serializer_t* net_writer) {
	sys_move_units = ecs_register_system(ecs, move_units, NULL, NULL, map);
	ecs_require_component(ecs, sys_move_units, id_comp_position);
	ecs_require_component(ecs, sys_move_units, id_comp_input);
	ecs_require_component(ecs, sys_move_units, id_comp_mover);

	sys_net_send_move =
		ecs_register_system(ecs, net_send_move, NULL, NULL, net_writer);
	ecs_require_component(ecs, sys_net_send_move, id_comp_net_move);
}

ecs_id_t sys_move_units;

ecs_ret_t move_units(ecs_t* ecs,
					 ecs_id_t* entities,
					 int entity_count,
					 ecs_dt_t dt,
					 void* udata) {
	map_t* map = udata;

	for(int i = 0; i < entity_count; i++) {
		ecs_id_t id				  = entities[i];
		comp_position_t* position = ecs_get(ecs, id, id_comp_position);
		comp_input_t* input		  = ecs_get(ecs, id, id_comp_input);
		comp_mover_t* mover		  = ecs_get(ecs, id, id_comp_mover);

		mover->_move_cooldown += dt * mover->movement_speed;
		Clamp(mover->_move_cooldown, 0.f, 1.0f);

		if(mover->_move_cooldown >= 1.0f) {
			bool is_moving = fabs(input->direction.x) > 0.f ||
							 fabs(input->direction.y) > 0.f;
			if(is_moving) {
				if(map_try_move(map,
								position->layer,
								id,
								position->grid_pos,
								input->direction)) {
					mover->from_tile = position->grid_pos;
					position->grid_pos =
						Vector2Add(position->grid_pos, input->direction);
					mover->_move_cooldown = 0;
					printf("moving\n");
					comp_net_move_t* net_move =
						ecs_add(ecs, id, id_comp_net_move, NULL);
					net_move->from_tile = mover->from_tile;
					net_move->to_tile	= position->grid_pos;
					net_move->entity	= id;
				}
			}
		}

		float percentage = Clamp(mover->_move_cooldown, 0.f, 1.f);

		Vector2 current_world_pos =
			map_grid_to_world_pos(map, position->layer, mover->from_tile);
		Vector2 target_world_pos =
			map_grid_to_world_pos(map, position->layer, position->grid_pos);

		position->value.x =
			Lerp(current_world_pos.x, target_world_pos.x, percentage);
		position->value.y =
			Lerp(current_world_pos.y, target_world_pos.y, percentage);

		//Vector2 result;
		//result.x = velocity->value.x * dt * mover->movement_speed;
		//result.y = velocity->value.y * dt * mover->movement_speed;

		//position->value = Vector2Add(position->value, result);
		//printf("position: %f, %f\n", position->value.x, position->value.y);
		//printf("tile_position: %f, %f\n",
		//position->grid_pos.x,
		//position->grid_pos.y);
	}
	return 0;
}

ecs_id_t sys_net_send_move;

ecs_ret_t net_send_move(ecs_t* ecs,
						ecs_id_t* entities,
						int entity_count,
						ecs_dt_t dt,
						void* udata) {

	(void)dt;
	serializer_t* net_writer = udata;

	for(int i = 0; i < entity_count; i++) {
		ecs_id_t id			  = entities[i];
		comp_net_move_t* move = ecs_get(ecs, id, id_comp_net_move);
		//TODO(risgrynsgrot)this should maybe be put into the serializer somehow
		net_write_byte(&net_writer->ser.net, COMPONENT_NET_MOVE, "type");
		ser_net_move(net_writer, move);
		printf("has net move: %i\n", ecs_has(ecs, id, id_comp_net_move));
		printf("has drawing: %i\n", ecs_has(ecs, id, id_comp_draw_sprite));
		ecs_queue_remove(ecs, id, id_comp_net_move);
	}

	return 0;
}

void net_send_move_trait(entities_t* entities,
						 trait_haver_t* trait,
						 serializer_t* net_writer) {
	for(int i = 0; i < trait->count; i++) {
		entity_t* entity		  = &trait->entity[i];
		comp_net_move_t* move = &entities->net_move_a[entity->id];
		net_write_byte(&net_writer->ser.net, COMPONENT_NET_MOVE, "type");
		ser_net_move(net_writer, move);

	}
	for(int i = 0; i < trait->count; i++) {
		entity_t* entity		  = &trait->entity[i];
		trait_entity_remove(trait, TRAIT_NET_MOVE, entity);
	}
}
