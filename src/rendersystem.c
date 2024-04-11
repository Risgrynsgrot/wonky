#include "rendersystem.h"
#include "components.h"
#include "raylib.h"

void register_render_systems(ecs_t* ecs) {
	sys_render_sprites =
		ecs_register_system(ecs, render_sprites, NULL, NULL, NULL);
}

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

		sprite->texture.width  = sprite->w;
		sprite->texture.height = sprite->h;
		DrawTexture(sprite->texture,
					position->value.x,
					position->value.y,
					sprite->color);
	}
	return 0;
}

void render_load_sprite(ecs_t* ecs, const char* path, ecs_id_t entity) {
	comp_draw_sprite_t* sprite = ecs_get(ecs, entity, id_comp_draw_sprite);

	sprite->texture = LoadTexture(path);
	sprite->w		= sprite->texture.width;
	sprite->h		= sprite->texture.height;
	sprite->color	= WHITE;
}
