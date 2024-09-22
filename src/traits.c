#include "traits.h"

void trait_entity_add(trait_havers_t* traits,
					  trait_types_e trait,
					  entity_t entity) {
	trait_haver_t* trait_haver				= &traits->has_trait[trait];
	trait_haver->entity[trait_haver->count] = entity;
	trait_haver->count++;
}

void trait_entity_remove(trait_havers_t* traits,
						 trait_types_e trait,
						 entity_t entity) {
	trait_haver_t* trait_haver = &traits->has_trait[trait];
	for(int i = 0; i < trait_haver->count; i++) {
		if(trait_haver->entity[i].id == entity.id &&
		   trait_haver->entity[i].generation == entity.generation) {
			trait_haver->entity[i] =
				trait_haver->entity[trait_haver->count - 1];
			trait_haver->count--;
			return;
		}
	}
}
