#include "components.h"

ecs_id_t id_comp_position;
ecs_id_t id_comp_rotation;
ecs_id_t id_comp_velocity;
ecs_id_t id_comp_input;
ecs_id_t id_comp_area_box;
ecs_id_t id_comp_col_box;
ecs_id_t id_comp_draw_sprite;
ecs_id_t id_comp_draw_box;
ecs_id_t id_comp_draw_circle;

void ecs_components_register(ecs_t* ecs) {
	ECS_REGISTER_COMPONENT(ecs, comp_position);
	ECS_REGISTER_COMPONENT(ecs, comp_rotation);
	ECS_REGISTER_COMPONENT(ecs, comp_velocity);
	ECS_REGISTER_COMPONENT(ecs, comp_input);
	ECS_REGISTER_COMPONENT(ecs, comp_area_box);
	ECS_REGISTER_COMPONENT(ecs, comp_col_box);
	ECS_REGISTER_COMPONENT(ecs, comp_draw_sprite);
	ECS_REGISTER_COMPONENT(ecs, comp_draw_box);
	ECS_REGISTER_COMPONENT(ecs, comp_draw_circle);
}
