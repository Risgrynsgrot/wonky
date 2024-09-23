#pragma once
#include "entity.h"

#define TRAITS_TYPE_ITER(_F, ...)                                              \
	_F(movable, MOVABLE, 0, NULL, NULL, __VA_ARGS__)                           \
	_F(drawable, DRAWABLE, 1, NULL, NULL, __VA_ARGS__)                         \
	_F(player_controlled, PLAYER_CONTROLLED, 2, NULL, NULL, __VA_ARGS__)

#define DECL_ENUM_TRAITS(lc, uc, i, ...) TRAIT_##uc = i,

typedef enum trait_types {
	TRAITS_TYPE_ITER(DECL_ENUM_TRAITS, void)
	TRAIT_COUNT
} trait_types_e;

typedef struct trait_haver {
	entity_t entity[MAX_ENTITY_COUNT];
	int count;
} trait_haver_t;

typedef struct trait_havers {
	trait_haver_t has_trait[TRAIT_COUNT];
} trait_havers_t;

void trait_entity_add(trait_havers_t* traits, trait_types_e trait, entity_t entity);
void trait_entity_remove(trait_havers_t* traits, trait_types_e trait, entity_t entity);
bool trait_entity_has(trait_havers_t* traits, trait_types_e trait, entity_t entity);
