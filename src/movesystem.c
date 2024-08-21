#include "movesystem.h"
#include "components.h"
#include "raymath.h"
#include <stdio.h>

void ecs_register_move_systems(ecs_t* ecs, map_t* map) {
	sys_move_units = ecs_register_system(ecs, move_units, NULL, NULL, map);
	ecs_require_component(ecs, sys_move_units, id_comp_position);
	ecs_require_component(ecs, sys_move_units, id_comp_input);
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
		comp_input_t* input		  = ecs_get(ecs, id, id_comp_input);
		comp_mover_t* mover		  = ecs_get(ecs, id, id_comp_mover);

		mover->_move_cooldown -= dt;
		Clamp(mover->_move_cooldown, 0.f, mover->movement_speed);

		if(mover->_move_cooldown <= 0) {
			bool is_moving = fabs(input->direction.x) > 0.f ||
							 fabs(input->direction.y) > 0.f;
			if(is_moving) {
				printf("trying to move!\n");
				if(map_try_move(map,
								position->layer,
								id,
								position->grid_pos,
								input->direction)) {
					mover->from_tile = position->grid_pos;
					position->grid_pos =
						Vector2Add(position->grid_pos, input->direction);
					mover->_move_cooldown = 1.f / mover->movement_speed;
				}
			}
		}

		float inverse_cooldown = 1.f / mover->movement_speed;
		float percentage =
			Clamp(inverse_cooldown - mover->_move_cooldown / inverse_cooldown,
				  0.f,
				  inverse_cooldown);

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
		printf("position: %f, %f\n", position->value.x, position->value.y);
	}
	return 0;
}
