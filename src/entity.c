#include "entity.h"

bool entity_has_components(entities_t* entities,
						   uint32_t id,
						   uint64_t component_bitset) {

	(void)entities;
	(void)id;
	(void)component_bitset;

	return true;
}

void entity_add_components(entities_t* entities,
						   uint32_t id,
						   uint64_t component_bitset) {
	(void)entities;
	(void)id;
	(void)component_bitset;
}

void entity_remove_components(entities_t* entities,
							  uint32_t id,
							  uint64_t component_bitset) {
	(void)entities;
	(void)id;
	(void)component_bitset;
}
