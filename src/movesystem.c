#include "movesystem.h"
#include "gameclient.h"

void move_players(client_t* client, float dt) {

	for(int i = 0; i < MAX_ENTITIES; i++) {
		component_type_t* mask		= client->components->c_mask;
		comp_position_t* positions	= client->components->position;

		if((mask[i] & (COMP_POSITION)) == 0) {
			continue;
		}
		positions[i].x += dt;
	}
}
