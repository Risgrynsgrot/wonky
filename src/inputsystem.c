#include "inputsystem.h"
#include "components.h"
#include "gameworld.h"

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
