#pragma once
#include "entity.h"
#include "map.h"

typedef struct gameworld gameworld_t;

void trait_move_units(gameworld_t* gameworld, float dt);

bool move_unit(gameworld_t* world,
			   entity_t entity,
			   comp_position_t* position,
			   comp_mover_t* mover,
			   Vector2 direction);
