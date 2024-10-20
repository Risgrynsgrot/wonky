#include "inputsystem.h"
#include "components.h"
#include "gameworld.h"
#include <stdio.h>

input_map_t input_init(void) {
	input_map_t result;
	result.up			  = KEY_UP;
	result.down			  = KEY_DOWN;
	result.left			  = KEY_LEFT;
	result.right		  = KEY_RIGHT;
	result.interact		  = KEY_Z;
	result.open_inventory = KEY_X;
	return result;
}

ecs_id_t sys_input_handle;

//ecs_id_t sys_input_move;

//void ecs_register_input_systems(ecs_t* ecs, input_map_t* keymap) {
//	sys_input_handle =
//		ecs_register_system(ecs, input_handle, NULL, NULL, keymap);
//	ecs_require_component(ecs, sys_input_handle, id_comp_input);
//
//	//sys_input_move = ecs_register_system(ecs, input_move, NULL, NULL, NULL);
//	//ecs_require_component(ecs, sys_input_move, id_comp_input);
//	//ecs_require_component(ecs, sys_input_move, id_comp_mover);
//}

//TODO(risgrynsgrot): REWORK THIS TO TRAIT
//ecs_ret_t input_handle(ecs_t* ecs,
//					   ecs_id_t* entities,
//					   int entity_count,
//					   ecs_dt_t dt,
//					   void* udata) {
//	(void)dt;
//	input_map_t* keymap = udata;
//
//	for(int i = 0; i < entity_count; i++) {
//		ecs_id_t id			= entities[i];
//		comp_input_t* input = ecs_get(ecs, id, id_comp_input);
//		int input_id		= input->input_id;
//
//		input->interact		  = IsKeyPressed(keymap[input_id].interact);
//		input->open_inventory = IsKeyPressed(keymap[input_id].open_inventory);
//
//		Vector2 direction;
//		direction.x = IsKeyDown(keymap[input_id].right) -
//					  IsKeyDown(keymap[input_id].left);
//		if(direction.x == 0.f) {
//			direction.y = IsKeyDown(keymap[input_id].down) -
//						  IsKeyDown(keymap[input_id].up);
//		}
//		input->direction = direction;
//	}
//	return 0;
//}

/*ecs_ret_t input_move(ecs_t* ecs,*/
/*					 ecs_id_t* entities,*/
/*					 int entity_count,*/
/*					 ecs_dt_t dt,*/
/*					 void* udata) {*/
/**/
/*	(void)dt;*/
/*	(void)udata;*/
/**/
/*	for(int i = 0; i < entity_count; i++) {*/
/*		ecs_id_t id				  = entities[i];*/
/*		comp_input_t* input		  = ecs_get(ecs, id, id_comp_input);*/
/*		comp_mover_t* mover = ecs_get(ecs, id, id_comp_mover);*/
/*		//printf("velocity: %f, %f\n", velocity->value.x, velocity->value.y);*/
/*	}*/
/*	return 0;*/
/*}*/

void trait_input_handle(gameworld_t* gameworld) {

	trait_haver_t* trait =
		&gameworld->traits.has_trait[TRAIT_PLAYER_CONTROLLED];
	entities_t* entities = &gameworld->entities;
	input_map_t* keymap	 = &gameworld->input_map[0]; //this should be handled per player

	for(int i = 0; i < trait->count; i++) {
		int id				= trait->entity[i].id;
		comp_input_t* input = &entities->input_a[id];
		int input_id		= input->input_id;

		input->interact		  = IsKeyPressed(keymap[input_id].interact);
		input->open_inventory = IsKeyPressed(keymap[input_id].open_inventory);

		Vector2 direction = {0};
		direction.x = IsKeyDown(keymap[input_id].right) -
					  IsKeyDown(keymap[input_id].left);
		if(direction.x == 0.f) {
			direction.y = IsKeyDown(keymap[input_id].down) -
						  IsKeyDown(keymap[input_id].up);
		}
		input->direction = direction;
	}
}
