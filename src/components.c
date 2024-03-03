#include "components.h"

void component_add(components_t* components, entity_t entity, component_type_t component) {
	components->c_mask[entity.id] |= component;
}
void component_remove(components_t* components, entity_t entity, component_type_t component) {
	components->c_mask[entity.id] &= ~component;
}
