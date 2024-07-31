#include "rendersystem.h"
#include "components.h"
#include "raylib.h"
#include <stdio.h>

void ecs_register_render_systems(ecs_t* ecs) {
	sys_render_sprites =
		ecs_register_system(ecs, render_sprites, NULL, NULL, NULL);
	ecs_require_component(ecs, sys_render_sprites, id_comp_position);
	ecs_require_component(ecs, sys_render_sprites, id_comp_draw_sprite);

	sys_render_boxes = ecs_register_system(ecs, render_boxes, NULL, NULL, NULL);
	ecs_require_component(ecs, sys_render_boxes, id_comp_position);
	ecs_require_component(ecs, sys_render_boxes, id_comp_draw_box);
}

ecs_id_t sys_render_sprites;

ecs_ret_t render_sprites(ecs_t* ecs,
						 ecs_id_t* entities,
						 int entity_count,
						 ecs_dt_t dt,
						 void* udata) {
	(void)dt;
	(void)udata;

	for(int i = 0; i < entity_count; i++) {
		ecs_id_t id				   = entities[i];
		comp_draw_sprite_t* sprite = ecs_get(ecs, id, id_comp_draw_sprite);
		comp_position_t* position  = ecs_get(ecs, id, id_comp_position);

		sprite->texture.width  = sprite->width;
		sprite->texture.height = sprite->height;
		DrawTexture(sprite->texture,
					position->value.x,
					position->value.y,
					sprite->color);
	}
	return 0;
}

ecs_id_t sys_render_boxes;

ecs_ret_t render_boxes(ecs_t* ecs,
					   ecs_id_t* entities,
					   int entity_count,
					   ecs_dt_t dt,
					   void* udata) {
	(void)dt;
	(void)udata;

	for(int i = 0; i < entity_count; i++) {
		ecs_id_t id				  = entities[i];
		comp_draw_box_t* box	  = ecs_get(ecs, id, id_comp_draw_box);
		comp_position_t* position = ecs_get(ecs, id, id_comp_position);

		DrawRectangleV(position->value,
					   (Vector2){.x = box->width, .y = box->height},
					   box->color);
	}
	return 0;
}

void render_load_sprite(ecs_t* ecs, const char* path, ecs_id_t entity) {
	comp_draw_sprite_t* sprite = ecs_add(ecs, entity, id_comp_draw_sprite, NULL);

	sprite->texture = LoadTexture(path);
	sprite->width	= sprite->texture.width;
	sprite->height	= sprite->texture.height;
	sprite->color	= WHITE;
	sprite->visible = true;
}
