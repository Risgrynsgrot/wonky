#include "entity.h"
#include <assert.h>

entity_t entity_new(entities_t* entities) {
	assert(entities->entity_count < MAX_ENTITY_COUNT);
	entity_t result = {.id		   = entities->unused[0],
					   .generation = entities->generation[result.id]};
	entities->entity_count++;
	int32_t unused_count = MAX_ENTITY_COUNT - entities->entity_count;
	entities->unused[0]	 = entities->unused[unused_count - 1];

	return result;
}

void entity_delete(entities_t* entities, entity_t entity) {
	assert(entities->entity_count > 0);
	if(entity.generation != entities->generation[entity.id]) {
		return; //generation doesn't match, so call is invalid
	}
	entities->generation[entity.id]++;
	entities->entity_count--;
	int32_t unused_count = MAX_ENTITY_COUNT - entities->entity_count;
	entities->unused[unused_count - 1] = entity.id;
}
