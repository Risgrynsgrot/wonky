#pragma once

#define ECS_DECL_SYSTEM(T) \
extern entity_t sys_##T;\
ecs_ret_t T\
(\
	ecs_t* ecs, entity_t* entities, int entity_count, ecs_dt_t dt, void* udata)
