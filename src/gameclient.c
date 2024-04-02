#include "gameclient.h"
#include "components.h"
#include "movesystem.h"
#include "rendersystem.h"
#include <raylib.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#	include <emscripten/emscripten.h>
#endif

int client_init(client_t* client) {
	client->quit	 = false;
	client->tickrate = 1.f / 64.f;
	entity_fill_empty(client);

	//if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
	//	fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
	//	return 1;
	//}

	//render_setup();

	client_raylib_init();

	client->components = malloc(sizeof(components_t));

	client->input_map[0] = input_init();

	entity_t entity = entity_new(client);
	component_add(client->components, entity, COMP_DRAW_SPRITE);
	component_add(client->components, entity, COMP_POSITION);
	component_add(client->components, entity, COMP_VELOCITY);
	component_add(client->components, entity, COMP_INPUT);
	comp_input_t* input = &client->components->input[entity.id];
	input->input_id		= 0;
	render_load_sprite(client, "test.png", entity);
	return 0;
}

void client_raylib_init(void) {
	InitWindow(800, 600, "Wonky!");
}

void client_start_loop(client_t* client) {
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop_arg(client_start_emscripten_loop, client, 0, 1);
	return;
#endif
	client_start_native_loop(client);
}

void client_start_native_loop(client_t* client) {
	while(!client->quit) {
		client_main_loop(client);
		client->quit |= WindowShouldClose();
	}
	client_deinit(client);
}

#if defined(PLATFORM_WEB)
void client_start_emscripten_loop(void* user_data) {
	client_t* client = (client_t*)user_data;
	client_main_loop(client);
}
#endif

void client_main_loop(client_t* client) {
	float lag		= 0.f;
	float deltaTime = GetFrameTime();
	lag += deltaTime;
	client_handle_input(client);
	// This needs to handle the difference between fps and
	// tickrate for justpressed events
	// maybe do input handling in normal update, then reset justpressed
	// after fixed update

	while(lag >= client->tickrate) {
		lag -= client->tickrate;
		// if(client->isclient) {
		client_update(client, client->tickrate);
		//input_reset(&client->input);
		// move this to game, so server can use it too
	}
	client_render(client);
}

void client_handle_input(client_t* client) {
	input_handle(client);
	input_move(client);
}

void client_update(client_t* client, float dt) {
	move_players(client, dt);
}

void client_render(client_t* client) {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	render_sprites(client);
	EndDrawing();
	//SDL_SetRenderDrawColor(client->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderClear(client->renderer);
	//render_sprites(client);
	//SDL_RenderPresent(client->renderer);
}

int client_deinit(client_t* client) {
	//SDL_DestroyWindow(client->window);
	//SDL_Quit();
	free(client->components);
	CloseWindow();
	return EXIT_SUCCESS;
}
