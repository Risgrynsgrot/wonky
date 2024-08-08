#include "movesystem.h"
#include "components.h"
#include "raymath.h"
#include <stdio.h>

void ecs_register_move_systems(ecs_t* ecs, map_t* map) {
	sys_move_units = ecs_register_system(ecs, move_units, NULL, NULL, map);
	ecs_require_component(ecs, sys_move_units, id_comp_position);
	ecs_require_component(ecs, sys_move_units, id_comp_velocity);
	ecs_require_component(ecs, sys_move_units, id_comp_mover);
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
		//comp_velocity_t* velocity = ecs_get(ecs, id, id_comp_velocity);
		comp_mover_t* mover = ecs_get(ecs, id, id_comp_mover);

		if(mover->_move_cooldown <= 0) {
			mover->_move_cooldown = 1.f / mover->movement_speed;
			Vector2 direction =
				Vector2Subtract(mover->target_tile, mover->current_tile);
			if(map_try_move(
				   map, position->layer, id, mover->current_tile, direction)) {
				position->grid_pos = mover->target_tile;
			}
		}

		float inverse_cooldown = 1.f / mover->movement_speed;
		float percentage =
			Clamp(inverse_cooldown - mover->_move_cooldown / inverse_cooldown,
				  0.f,
				  inverse_cooldown);
		position->value.x =
			Lerp(mover->current_tile.x, mover->target_tile.x, percentage);
		position->value.y =
			Lerp(mover->current_tile.y, mover->target_tile.y, percentage);

		//Vector2 result;
		//result.x = velocity->value.x * dt * mover->movement_speed;
		//result.y = velocity->value.y * dt * mover->movement_speed;

		//position->value = Vector2Add(position->value, result);
		//printf("position: %f, %f", position->value.x, position->value.y);
	}
	return 0;
}
