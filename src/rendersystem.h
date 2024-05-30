#pragma once
#include "pico_ecs.h"
#include "systems.h"

typedef struct client client_t;
typedef struct entity entity_t;


void ecs_register_render_systems(ecs_t* ecs);
ECS_DECL_SYSTEM(render_sprites);
ECS_DECL_SYSTEM(render_boxes);

void render_load_sprite(ecs_t* ecs, const char* path, ecs_id_t entity);
