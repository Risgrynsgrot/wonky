#include "inputsystem.h"
#include "components.h"
#include "gameclient.h"

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

ecs_ret_t input_handle(ecs_t* ecs,
					   ecs_id_t* entities,
					   int entity_count,
					   ecs_dt_t dt,
					   void* udata) {
	(void)dt;
	input_map_t* keymap = udata;

	for(int i = 0; i < entity_count; i++) {
		ecs_id_t id			= entities[i];
		comp_input_t* input = ecs_get(ecs, id, id_comp_input);

		Vector2 direction;
		int input_id = input->input_id;
		direction.x	 = IsKeyDown(keymap[input_id].right) -
					  IsKeyDown(keymap[input_id].left);
		direction.y =
			IsKeyDown(keymap[input_id].down) - IsKeyDown(keymap[input_id].up);
		input->direction	  = direction;
		input->interact		  = IsKeyPressed(keymap[input_id].interact);
		input->open_inventory = IsKeyPressed(keymap[input_id].open_inventory);
	}
	return 0;
}

ecs_ret_t input_move(ecs_t* ecs,
					 ecs_id_t* entities,
					 int entity_count,
					 ecs_dt_t dt,
					 void* udata) {

	(void)dt;
	(void)udata;

	for(int i = 0; i < entity_count; i++) {
		ecs_id_t id = entities[i];
		comp_input_t* input		= ecs_get(ecs, id, id_comp_input);
		comp_velocity_t* velocity		= ecs_get(ecs, id, id_comp_velocity);

		velocity->value = input->direction;
	}
	return 0;
}
