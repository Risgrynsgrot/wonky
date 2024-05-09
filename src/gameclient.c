#include "gameclient.h"
#include "components.h"
#include "luajit.h"
#include "map.h"
#include "movesystem.h"
#include "rendersystem.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define PICO_ECS_MAX_SYSTEMS 16
#define PICO_ECS_MAX_COMPONENTS 64
#define PICO_ECS_IMPLEMENTATION
#include "pico_ecs.h"

#if defined(PLATFORM_WEB)
#	include <emscripten/emscripten.h>
#endif

int client_init(client_t* client) {
	client->quit	 = false;
	client->tickrate = 1.f / 64.f;
	client->lag		 = 0.f;

	//render_setup();

	client_raylib_init();

	client->input_map[0] = input_init();

	ldtk_map_t map;
	map_load_ldtk("assets/levels/testgym.ldtk", &map);

						 client->ecs = ecs_new(100, NULL);
	ecs_components_register(client->ecs);
	ecs_register_render_systems(client->ecs);
	ecs_register_input_systems(client->ecs, client->input_map);
	ecs_register_move_systems(client->ecs);

	ecs_id_t entity = ecs_create(client->ecs);
	ecs_add(client->ecs, entity, id_comp_position, NULL);
	ecs_add(client->ecs, entity, id_comp_velocity, NULL);
	ecs_add(client->ecs, entity, id_comp_draw_sprite, NULL);
	ecs_add(client->ecs, entity, id_comp_input, NULL);

	comp_input_t* input = ecs_get(client->ecs, entity, id_comp_input);
	input->input_id		= 0;
	render_load_sprite(client->ecs, "assets/test.png", entity);
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
	float deltaTime = GetFrameTime();
	client->lag += deltaTime;
	client_handle_input(client);
	//printf("Tickrate: %f\n", client->tickrate);
	//printf("lag: %f\n", client->lag);
	// This needs to handle the difference between fps and
	// tickrate for justpressed events
	// maybe do input handling in normal update, then reset justpressed
	// after fixed update

	while(client->lag >= client->tickrate) {
		client->lag -= client->tickrate;
		// if(client->isclient) {
		printf("Update!\n");
		client_update(client, client->tickrate);
		//input_reset(&client->input);
		// move this to game, so server can use it too
	}
	client_render(client);
}

void client_handle_input(client_t* client) {
	ecs_update_system(client->ecs, sys_input_handle, 0.f);
	ecs_update_system(client->ecs, sys_input_move, 0.f);
}

void client_update(client_t* client, float dt) {
	ecs_update_system(client->ecs, sys_move_players, dt);
}

void client_render(client_t* client) {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	ecs_update_system(client->ecs, sys_render_sprites, 0.f);
	EndDrawing();
	//SDL_SetRenderDrawColor(client->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderClear(client->renderer);
	//render_sprites(client);
	//SDL_RenderPresent(client->renderer);
}

int client_deinit(client_t* client) {
	//SDL_DestroyWindow(client->window);
	//SDL_Quit();
	ecs_free(client->ecs);
	CloseWindow();
	return EXIT_SUCCESS;
}
