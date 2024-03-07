#include "rendersystem.h"
#include "components.h"
#include "gameclient.h"
#include "raylib.h"

void render_sprites(client_t* client) {
	for(int i = 0; i < MAX_ENTITIES; i++) {
		component_type_t* mask		= client->components->c_mask;
		comp_draw_sprite_t* sprites = client->components->draw_sprite;
		comp_position_t* positions	= client->components->position;

		if((mask[i] & (COMP_DRAW_SPRITE | COMP_POSITION)) == 0) {
			continue;
		}
		sprites[i].texture.width = sprites[i].w;
		sprites[i].texture.height = sprites[i].h;
		DrawTexture(sprites[i].texture,
					positions[i].value.x,
					positions[i].value.y,
					sprites[i].color);
	}
}

void render_load_sprite(client_t* client, const char* path, entity_t entity) {
	comp_draw_sprite_t* sprite = &client->components->draw_sprite[entity.id];

	sprite->texture = LoadTexture(path);
	sprite->w = sprite->texture.width;
	sprite->h = sprite->texture.height;
	sprite->color = WHITE;
}
