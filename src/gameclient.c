#include "gameclient.h"
#include "components.h"
#include "movesystem.h"
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

int client_init(client_t* client) {
	client->quit	  = false;
	client->tickrate  = 1.f / 64.f;
	entity_fill_empty(client);

	//if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
	//	fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
	//	return 1;
	//}

	//render_setup();
	
	client_raylib_init();

	client->components = malloc(sizeof(components_t));

	entity_t entity = entity_new(client);
	component_add(client->components, entity, COMP_DRAW_SPRITE);
	component_add(client->components, entity, COMP_POSITION);
	return 0;
}

void client_raylib_init(void) {
	InitWindow(800, 600, "Wonky!");
}

void client_start_loop(client_t* client) {
	float lag = 0.f;

	while(!client->quit) {
		float deltaTime = GetFrameTime();
		lag += deltaTime;
		//client_handle_input(client);
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
		client_render();
		client->quit |= WindowShouldClose();
	}
}

//void client_handle_input(client_t* client) {
//	//printf("%f", client->tickrate);
//	SDL_Event event;
//	while(SDL_PollEvent(&event)) {
//		switch(event.type) {
//		case SDL_QUIT:
//			client->quit = true;
//			break;
//		case SDL_KEYDOWN:
//			input_update_keydown(&client->input, &event.key);
//			break;
//		case SDL_KEYUP:
//			input_update_keyup(&client->input, &event.key);
//			break;
//		}
//	}
//}

void client_update(client_t* client, float dt) {
	move_players(client, dt);
}

void client_render(void) {
	BeginDrawing();
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
