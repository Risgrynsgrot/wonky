#pragma once
#include "pico_ecs.h"

ecs_ret_t col_box_box(
	ecs_t* ecs, ecs_id_t* entities, int entity_count, ecs_dt_t dt, void* udata);
