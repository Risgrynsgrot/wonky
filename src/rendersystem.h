#pragma once
#include "pico_ecs.h"
#include "systems.h"

void ecs_register_render_systems(ecs_t* ecs);
ECS_DECL_SYSTEM(render_sprites);
ECS_DECL_SYSTEM(render_boxes);

typedef enum asset_type {
	ASSET_TYPE_SPRITE,
} asset_type_e;

void render_load_sprite(ecs_t* ecs, const char* path, ecs_id_t entity);
