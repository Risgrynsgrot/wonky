#include "movesystem.h"
#include "components.h"

ecs_ret_t move_players(ecs_t* ecs,
					   ecs_id_t* entities,
					   int entity_count,
					   ecs_dt_t dt,
					   void* udata) {
	(void)udata;

	for(int i = 0; i < entity_count; i++) {
		ecs_id_t id				   = entities[i];
		comp_position_t* position = ecs_get(ecs, id, id_comp_position);
		comp_velocity_t* velocity = ecs_get(ecs, id, id_comp_velocity);

		Vector2 result;
		result.x = velocity->value.x * dt * 1000;
		result.y = velocity->value.y * dt * 1000;

		position->value = Vector2Add(position->value, result);
	}
}
