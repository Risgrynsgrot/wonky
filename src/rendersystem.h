#pragma once
//#include "systems.h"

typedef struct gameworld gameworld_t;
typedef struct entity entity_t;

//void ecs_register_render_systems(ecs_t* ecs);
//ECS_DECL_SYSTEM(render_sprites);
//ECS_DECL_SYSTEM(render_boxes);

void trait_render_sprites(gameworld_t* gameworld);
void trait_render_boxes(gameworld_t* gameworld);

typedef enum asset_type {
	ASSET_TYPE_SPRITE,
} asset_type_e;

void render_load_sprite(gameworld_t* world, const char* path, entity_t entity);
