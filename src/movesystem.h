#pragma once
#include "systems.h"
#include "pico_ecs.h"

typedef struct client client_t;

void ecs_register_move_systems(ecs_t* ecs);
ECS_DECL_SYSTEM(move_players);
