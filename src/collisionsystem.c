#include "collisionsystem.h"
#include "components.h"

//TODO(risgrynsgrot) if I'm doing this more pokemon like, I barely need
//real col checks.

ecs_ret_t col_box_box(ecs_t* ecs,
					   ecs_id_t* entities,
					   int entity_count,
					   ecs_dt_t dt,
					   void* udata) {

	(void)dt;
	(void)udata;

	for(int i = 0; i < entity_count; i++) {
		ecs_id_t id				  = entities[i];
		comp_col_box_t* box	  = ecs_get(ecs, id, id_comp_draw_box);
		comp_position_t* position = ecs_get(ecs, id, id_comp_position);
		comp_velocity_t* velocity = ecs_get(ecs, id, id_comp_velocity);


	}
	return 0;
}
