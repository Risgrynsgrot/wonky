#pragma once
#include "components.h"

#define DECL_COMPONENT_SOA(lc, uc, i, ...)                                     \
	comp_##lc##_t lc##_a[COMPONENT_COUNT];

//This is if I decide to do megastruct instead of ecs
typedef struct entities {
	ECS_COMPONENTS_TYPE_ITER(DECL_COMPONENT_SOA, void)
	uint64_t components;
	int entity_count;
} entities_t;

bool entity_has_components(entities_t* entities,
						   uint32_t id,
						   uint64_t component_bitset);
void entity_add_components(entities_t* entities,
						   uint32_t id,
						   uint64_t component_bitset);
void entity_remove_components(entities_t* entities,
							  uint32_t id,
							  uint64_t component_bitset);
