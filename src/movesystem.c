#include "movesystem.h"
#include "gameclient.h"
#include "raymath.h"

void move_players(client_t* client, float dt) {

	for(int i = 0; i < MAX_ENTITIES; i++) {
		component_type_t* mask		= client->components->c_mask;
		comp_position_t* positions	= client->components->position;
		comp_velocity_t* velocities = client->components->velocity;

		if((mask[i] & (COMP_POSITION | COMP_VELOCITY)) == 0) {
			continue;
		}
		Vector2 result;
		result.x = velocities[i].value.x * dt * 1000;
		result.y = velocities[i].value.y * dt * 1000;

		positions[i].value =
			Vector2Add(positions[i].value, result);
	}
}
