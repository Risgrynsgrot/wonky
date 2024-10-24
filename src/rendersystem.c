#include "rendersystem.h"
#include "components.h"
#include "gameworld.h"
#include "raylib.h"

//void ecs_register_render_systems(ecs_t* ecs) {
//	sys_render_sprites =
//		ecs_register_system(ecs, render_sprites, NULL, NULL, NULL);
//	ecs_require_component(ecs, sys_render_sprites, id_comp_position);
//	ecs_require_component(ecs, sys_render_sprites, id_comp_draw_sprite);
//
//	sys_render_boxes = ecs_register_system(ecs, render_boxes, NULL, NULL, NULL);
//	ecs_require_component(ecs, sys_render_boxes, id_comp_position);
//	ecs_require_component(ecs, sys_render_boxes, id_comp_draw_box);
//}

void trait_render_sprites(gameworld_t* gameworld) {

	trait_haver_t* trait = &gameworld->traits.has_trait[TRAIT_DRAW_SPRITE];
	entities_t* entities = &gameworld->entities;

	for(int i = 0; i < trait->count; i++) {
		uint32_t id				   = trait->entity[i].id;
		comp_draw_sprite_t* sprite = &entities->draw_sprite_a[id];
		comp_position_t* position  = &entities->position_a[id];

		sprite->texture.width  = sprite->width;
		sprite->texture.height = sprite->height;
		DrawTexture(sprite->texture,
					position->value.x,
					position->value.y,
					sprite->color);
	}
}

//ecs_id_t sys_render_sprites;
//ecs_ret_t render_sprites(ecs_t* ecs,
//						 ecs_id_t* entities,
//						 int entity_count,
//						 ecs_dt_t dt,
//						 void* udata) {
//	(void)dt;
//	(void)udata;
//
//	for(int i = 0; i < entity_count; i++) {
//		ecs_id_t id				   = entities[i];
//		comp_draw_sprite_t* sprite = ecs_get(ecs, id, id_comp_draw_sprite);
//		comp_position_t* position  = ecs_get(ecs, id, id_comp_position);
//
//		sprite->texture.width  = sprite->width;
//		sprite->texture.height = sprite->height;
//		DrawTexture(sprite->texture,
//					position->value.x,
//					position->value.y,
//					sprite->color);
//	}
//	return 0;
//}

//ecs_id_t sys_render_boxes;

//ecs_ret_t render_boxes(ecs_t* ecs,
//					   ecs_id_t* entities,
//					   int entity_count,
//					   ecs_dt_t dt,
//					   void* udata) {
//	(void)dt;
//	(void)udata;
//
//	for(int i = 0; i < entity_count; i++) {
//		ecs_id_t id				  = entities[i];
//		comp_draw_box_t* box	  = ecs_get(ecs, id, id_comp_draw_box);
//		comp_position_t* position = ecs_get(ecs, id, id_comp_position);
//
//		DrawRectangleV(position->value,
//					   (Vector2){.x = box->width, .y = box->height},
//					   box->color);
//	}
//	return 0;
//}

void trait_render_boxes(gameworld_t* gameworld) {
	trait_haver_t* trait = &gameworld->traits.has_trait[TRAIT_DRAW_BOX];
	entities_t* entities = &gameworld->entities;

	for(int i = 0; i < trait->count; i++) {
		uint32_t id				  = trait->entity[i].id;
		comp_position_t* position = &entities->position_a[id];
		comp_draw_box_t* box	  = &entities->draw_box_a[id];

		DrawRectangleV(position->value,
					   (Vector2){.x = box->width, .y = box->height},
					   box->color);
	}
}

void render_load_sprite(gameworld_t* world, const char* path, entity_t entity) {
	trait_entity_add(&world->traits, TRAIT_DRAW_SPRITE, entity);
	comp_draw_sprite_t* sprite =
		&world->entities.draw_sprite_a[entity.id];

	sprite->texture = LoadTexture(path);
	sprite->width	= sprite->texture.width;
	sprite->height	= sprite->texture.height;
	sprite->color	= WHITE;
	sprite->visible = true;
}
