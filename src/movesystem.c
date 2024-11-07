#include "movesystem.h"
#include "components.h"
#include "gameworld.h"
#include "net_data.h"
#include "network_common.h"
#include "raymath.h"

void trait_move_units(gameworld_t* world, float dt) {
	trait_haver_t* trait	 = &world->traits.has_trait[TRAIT_MOVABLE];
	entities_t* entities	 = &world->entities;
	map_t* map				 = &world->map;
	serializer_t* net_writer = &world->net_writer;

	for(int i = 0; i < trait->count; i++) {
		entity_t entity			  = trait->entity[i];
		comp_position_t* position = &entities->position_a[entity.id];
		comp_input_t* input		  = &entities->input_a[entity.id];
		comp_mover_t* mover		  = &entities->mover_a[entity.id];

		mover->_move_cooldown += dt * mover->movement_speed;
		Clamp(mover->_move_cooldown, 0.f, 1.0f);

		if(mover->_move_cooldown >= 1.0f) {
			bool is_moving = fabs(input->direction.x) > 0.f ||
							 fabs(input->direction.y) > 0.f;
			if(is_moving) {
				if(move_unit(
					   world, entity, position, mover, input->direction)) {
					net_move_t net_move = {
						.from_tile = mover->from_tile,
						.to_tile   = position->grid_pos,
						.entity_id = entity.id,
					};
					net_write_byte(&net_writer->ser.net, NET_MOVE, "type");
					ser_net_move(net_writer, &net_move);
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
	}
}

bool move_unit(gameworld_t* world,
			   entity_t entity,
			   comp_position_t* position,
			   comp_mover_t* mover,
			   Vector2 direction) {
	map_t* map = &world->map;
	if(!map_try_move(
		   map, position->layer, entity, position->grid_pos, direction)) {
		return false;
	}
	mover->from_tile	  = position->grid_pos;
	position->grid_pos	  = Vector2Add(position->grid_pos, direction);
	mover->_move_cooldown = 0;
	return true;
}
