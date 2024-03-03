#pragma once

typedef struct client client_t;
typedef struct entity entity_t;

void render_sprites(client_t* client);

void render_load_sprite(client_t* client, const char* path, entity_t entity);
