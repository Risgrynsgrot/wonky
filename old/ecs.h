#pragma once
//#include "components.h"
//
//struct ecs_manager {
//	int test;
//};
//
//#define COMPONENT_TYPES_ITER(_F) \
//	_F(comp_position, "POSITION", "position", 0) \
//	_F(comp_rotation, "ROTATION", "rotation", 1)
//
//#define COMPONENT_CREATE_POOLS(T, ...) \
//	typedef struct T ## _pool { \
//		T##_t instances[MAX_ENTITIES]; \
//		int count; \
//	} T##_pool_t;
//
//COMPONENT_TYPES_ITER(COMPONENT_CREATE_POOLS)
//
//typedef struct component_pools {
//#define DECL_COMPONENT_POOLS(T, ...) T##_t T##s;
//	COMPONENT_TYPES_ITER(DECL_COMPONENT_POOLS)
//} component_pools_t;
//
//
////void move(comp_position_t* positions, comp_rotation_t* rotations) {
//
////}
