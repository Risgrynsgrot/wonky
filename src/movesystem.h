#pragma once
#include "systems.h"
#include "pico_ecs.h"
#include "map.h"

typedef struct gameworld client_t;

void ecs_register_move_systems(ecs_t* ecs, map_t* map, serializer_t* net_writer);
ECS_DECL_SYSTEM(move_units);
ECS_DECL_SYSTEM(net_send_move);
