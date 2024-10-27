#pragma once

#include "components.h"
#include <stdint.h>
#define MAX_ENTITY_COUNT 1024 //this can be increased when needed

typedef struct entity {
	int32_t id;
	uint32_t generation;
} entity_t;

#define DECL_COMPONENT_SOA(lc, uc, i, ...)                                     \
	comp_##lc##_t lc##_a[MAX_ENTITY_COUNT];

//This is if I decide to do megastruct instead of ecs
typedef struct entities {
	ECS_COMPONENTS_TYPE_ITER(DECL_COMPONENT_SOA, void)
	uint64_t components;
	int32_t entity_count;
	uint32_t unused[MAX_ENTITY_COUNT];
	uint32_t generation[MAX_ENTITY_COUNT];
} entities_t;

void entities_init(entities_t* entities);
entity_t entity_new(entities_t* entities);
void entity_set_type(entities_t* entities, entity_t entity, const char* type);
const char* entity_get_type(entities_t* entities, entity_t entity);
void entity_delete(entities_t* entities, entity_t entity);

/*
 * Do sparse set based on conditions instead of component sets
 * for example:
 * has events to send
 * is flammable (flam comp, pos comp)
 * is playable character
 * controllable
 * so it's more like a trait thing
 *
 */
