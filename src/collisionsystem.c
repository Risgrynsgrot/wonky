//#include "collisionsystem.h"
//#include "components.h"
#include "stdio.h"

void test(void) {
	printf("bruh");
}

//TODO(risgrynsgrot) if I'm doing this more pokemon like, I barely need
//real col checks.

//ecs_id_t sys_col_box_map;
//void register_collision_systems(ecs_t* ecs, ldtk_map_t* map) {
	//sys_col_box_map = ecs_register_system(ecs, col_box_map, NULL, NULL, map);
//}
//ecs_ret_t col_box_box(ecs_t* ecs,
//					  ecs_id_t* entities,
//					  int entity_count,
//					  ecs_dt_t dt,
//					  void* udata) {
//
//	(void)dt;
//	(void)udata;
//
//	for(int i = 0; i < entity_count; i++) {
//		ecs_id_t id				  = entities[i];
//		comp_col_box_t* box		  = ecs_get(ecs, id, id_comp_draw_box);
//		comp_position_t* position = ecs_get(ecs, id, id_comp_position);
//		comp_velocity_t* velocity = ecs_get(ecs, id, id_comp_velocity);
//	}
//	return 0;
//}
//
//ecs_ret_t col_box_map(ecs_t* ecs,
//					  ecs_id_t* entities,
//					  int entity_count,
//					  ecs_dt_t dt,
//					  void* udata) {
//
//	(void)dt;
//	ldtk_map_t* map = udata;
//	for(int i = 0; i < entity_count; i++) {
//		ecs_id_t id				  = entities[i];
//		comp_col_box_t* box		  = ecs_get(ecs, id, id_comp_draw_box);
//		comp_position_t* position = ecs_get(ecs, id, id_comp_position);
//		comp_velocity_t* velocity = ecs_get(ecs, id, id_comp_velocity);
//
//	}
//
//	return 0;
//}
