#include "inputsystem.h"
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

void input_handle(client_t* client) {
	input_map_t* keymap = client->input_map;

	for(int i = 0; i < MAX_ENTITIES; i++) {
		component_type_t* mask = client->components->c_mask;
		comp_input_t* inputs   = client->components->input;

		if((mask[i] & (COMP_INPUT)) == 0) {
			continue;
		}
		Vector2 direction;
		int input_id = inputs[i].input_id;
		direction.x = IsKeyDown(keymap[input_id].right) - IsKeyDown(keymap[input_id].left);
		direction.y = IsKeyDown(keymap[input_id].down) - IsKeyDown(keymap[input_id].up);
		inputs[i].direction = direction;
		inputs[i].interact = IsKeyPressed(keymap[input_id].interact);
		inputs[i].open_inventory = IsKeyPressed(keymap[input_id].open_inventory);
	}
}

void input_move(client_t* client) {

	for(int i = 0; i < MAX_ENTITIES; i++) {
		component_type_t* mask = client->components->c_mask;
		comp_input_t* inputs   = client->components->input;
		comp_velocity_t* velocities   = client->components->velocity;

		if((mask[i] & (COMP_INPUT | COMP_VELOCITY)) == 0) {
			continue;
		}

		velocities[i].value = inputs[i].direction;
	}
}
