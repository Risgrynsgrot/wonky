#pragma once

#define ECS_DECL_SYSTEM(T) \
extern int sys_##T;\
ecs_ret_t T\
(\
	ecs_t* ecs, ecs_id_t* entities, int entity_count, ecs_dt_t dt, void* udata)
