#pragma once

struct ecs_manager {

};


#define COMPONENT_REGISTER(T) \
	typedef struct T ## _pool { \
		T ## [MAX_ENTITIES];\
		int count;
	} T ## _pool_t\


#define MAX_ENTITIES 1000

typedef struct components {
	comp_position_t position[MAX_ENTITIES];
	comp_rotation_t rotation[MAX_ENTITIES];
	comp_velocity_t velocity[MAX_ENTITIES];
	comp_input_t input[MAX_ENTITIES];
	comp_area_box_t area_box[MAX_ENTITIES];
	comp_col_box_t col_box[MAX_ENTITIES];
	comp_draw_sprite_t draw_sprite[MAX_ENTITIES];
	comp_draw_box_t draw_box[MAX_ENTITIES];
	comp_draw_circle_t draw_circle[MAX_ENTITIES];
	//comp_draw_text_t draw_text[MAX_ENTITIES];
	component_type_t c_mask[MAX_ENTITIES];
} components_t;

void move(comp_position_t* positions, comp_rotation_t* rotations) {

	COMPONENT_REGISTER(comp_position_t);
}
