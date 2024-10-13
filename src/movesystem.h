#pragma once
#include "entity.h"
#include "map.h"
#include "pico_ecs.h"
#include "systems.h"
#include "traits.h"

typedef struct gameworld gameworld_t;

void ecs_register_move_systems(ecs_t* ecs,
							   map_t* map,
							   serializer_t* net_writer);
ECS_DECL_SYSTEM(move_units);
ECS_DECL_SYSTEM(net_send_move);

void trait_move_units(gameworld_t* gameworld, float dt);
