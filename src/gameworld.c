#include "gameworld.h"
#include "components.h"
#include "lua.h"
#include "map.h"
#include "movesystem.h"
#include "rendersystem.h"
#include "scriptloader.h"
#include <lauxlib.h>
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

int gameworld_init(gameworld_t* gameworld) {
	gameworld->quit	 = false;
	gameworld->tickrate = 1.f / 64.f;
	gameworld->lag		 = 0.f;

	//render_setup();

	gameworld_raylib_init();

	gameworld->input_map[0] = input_init();

	printf("bruh\n");
	//test_func();

	gameworld->ecs = ecs_new(100, NULL);
	ecs_components_register(gameworld->ecs);
	ecs_register_render_systems(gameworld->ecs);
	ecs_register_input_systems(gameworld->ecs, gameworld->input_map);

	map_new("assets/levels/testgym.ldtk", &gameworld->map);
	//map_load_ldtk("assets/levels/testgym.ldtk", &gameworld->map.data);
	ldtk_map_t* map = &gameworld->map.data;
	ecs_register_move_systems(gameworld->ecs, &gameworld->map);
	//TODO(risgrynsgrot) these should be spawned using their type instead of id
	//except for maybe special cases
	ldtk_layer_t* entity_layer = level_get_layer(&map->levels[0], "entities");
	if(entity_layer != NULL) {
		level_spawn_entities(entity_layer, gameworld->ecs);
	}
	ldtk_layer_t* int_layer = level_get_layer(&map->levels[0], "intgrid");
	if(int_layer != NULL) {
		printf("spawning terrain\n");
		level_spawn_terrain(int_layer, gameworld->ecs);
	}

	ecs_register_move_systems(gameworld->ecs, &gameworld->map);

	ecs_id_t entity = ecs_create(gameworld->ecs);

	lua_State* L = script_lua_init();
	lua_pushlightuserdata(L, gameworld->ecs);
	lua_setglobal(L, "ecs");
	ecs_lua_register_module(L);

	script_load(L, "assets/scripts/luatest.lua");

	lua_getglobal(L, "Luatest");
	if(lua_istable(L, -1)) {
		lua_getfield(L, -1, "onCreate");
		lua_pushinteger(L, entity);
		printf("entityID: %d\n", entity);
		if(lua_pcall(L, 1, 0, 0) != LUA_OK) {
			luaL_error(L, "Error: %s\n", lua_tostring(L, -1));
		}
		//lua_pop(L, 2);

		lua_getfield(L, -1, "testfunc");
		lua_pushinteger(L, entity);
		printf("entityID: %d\n", entity);
		if(lua_pcall(L, 1, 0, 0) != LUA_OK) {
			luaL_error(L, "Error: %s\n", lua_tostring(L, -1));
		}
		lua_pop(L, 2);
	} else {
		printf("uuuh\n");
	}

	script_lua_close(L);

	comp_position_t* position = ecs_get(gameworld->ecs, entity, id_comp_position);
	printf("placing entity at %f, %f", position->grid_pos.x, position->grid_pos.y);
	map_add_entity(&gameworld->map, 0, position->grid_pos, entity);

	return 0;
}

void gameworld_raylib_init(void) {
	InitWindow(1280, 720, "Wonky!");
}

void gameworld_start_loop(gameworld_t* gameworld) {
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop_arg(gameworld_start_emscripten_loop, gameworld, 0, 1);
	return;
#endif
	gameworld_start_native_loop(gameworld);
}

void gameworld_start_native_loop(gameworld_t* gameworld) {
	while(!gameworld->quit) {
		gameworld_main_loop(gameworld);
		gameworld->quit |= WindowShouldClose();
	}
	gameworld_deinit(gameworld);
}

#if defined(PLATFORM_WEB)
void gameworld_start_emscripten_loop(void* user_data) {
	gameworld_t* gameworld = (gameworld_t*)user_data;
	gameworld_main_loop(gameworld);
}
#endif

void gameworld_main_loop(gameworld_t* gameworld) {
	float deltaTime = GetFrameTime();
	gameworld->lag += deltaTime;
	gameworld_handle_input(gameworld);
	//printf("Tickrate: %f\n", gameworld->tickrate);
	//printf("lag: %f\n", gameworld->lag);
	// This needs to handle the difference between fps and
	// tickrate for justpressed events
	// maybe do input handling in normal update, then reset justpressed
	// after fixed update

	while(gameworld->lag >= gameworld->tickrate) {
		gameworld->lag -= gameworld->tickrate;
		// if(gameworld->isgameworld) {
		gameworld_update(gameworld, gameworld->tickrate);
		//input_reset(&gameworld->input);
	}
	gameworld_render(gameworld);
}

void gameworld_handle_input(gameworld_t* gameworld) {
	ecs_update_system(gameworld->ecs, sys_input_handle, 0.f);
	//ecs_update_system(gameworld->ecs, sys_input_move, 0.f);
}

void gameworld_update(gameworld_t* gameworld, float dt) {
	ecs_update_system(gameworld->ecs, sys_move_units, dt);
}

void gameworld_render(gameworld_t* gameworld) {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	ecs_update_system(gameworld->ecs, sys_render_sprites, 0.f);
	ecs_update_system(gameworld->ecs, sys_render_boxes, 0.f);
	EndDrawing();
	//SDL_SetRenderDrawColor(gameworld->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderClear(gameworld->renderer);
	//render_sprites(gameworld);
	//SDL_RenderPresent(gameworld->renderer);
}

int gameworld_deinit(gameworld_t* gameworld) {
	//SDL_DestroyWindow(gameworld->window);
	//SDL_Quit();
	ecs_free(gameworld->ecs);
	CloseWindow();
	return EXIT_SUCCESS;
}
