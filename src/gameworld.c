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
#include <sys/time.h>

#define PICO_ECS_MAX_SYSTEMS 16
#define PICO_ECS_MAX_COMPONENTS 64
#define PICO_ECS_IMPLEMENTATION
#include "pico_ecs.h"

#if defined(PLATFORM_WEB)
#	include <emscripten/emscripten.h>
#endif

int gameworld_init(gameworld_t* world, bool headless) {
	world->quit		= false;
	world->tickrate = 1.f / 64.f;
	world->lag		= 0.f;
	world->headless = headless;
	world->net_writer = new_writer_network((ser_net_t){0});

	//render_setup();

	if(!world->headless) {
		gameworld_raylib_init();
	}

	world->input_map[0] = input_init();

	printf("bruh\n");
	//test_func();

	world->ecs = ecs_new(100, NULL);
	ecs_components_register(world->ecs);
	ecs_register_render_systems(world->ecs);
	ecs_register_input_systems(world->ecs, world->input_map);

	map_new("assets/levels/testgym.ldtk", &world->map);
	//map_load_ldtk("assets/levels/testgym.ldtk", &world->map.data);
	ldtk_map_t* map = &world->map.data;
	//TODO(risgrynsgrot) these should be spawned using their type instead of id
	//except for maybe special cases
	ldtk_layer_t* entity_layer = level_get_layer(&map->levels[0], "entities");
	if(entity_layer != NULL) {
		level_spawn_entities(entity_layer, world->ecs);
	}
	ldtk_layer_t* int_layer = level_get_layer(&map->levels[0], "intgrid");
	if(int_layer != NULL) {
		printf("spawning terrain\n");
		level_spawn_terrain(int_layer, world->ecs);
	}

	ecs_register_move_systems(world->ecs, &world->map, &world->net_writer);

	ecs_id_t entity = ecs_create(world->ecs);

	lua_State* L = script_lua_init();
	lua_pushlightuserdata(L, world->ecs);
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

	comp_position_t* position =
		ecs_get(world->ecs, entity, id_comp_position);
	printf(
		"placing entity at %f, %f", position->grid_pos.x, position->grid_pos.y);
	map_add_entity(&world->map, 0, position->grid_pos, entity);

	return 0;
}

void gameworld_raylib_init(void) {
	InitWindow(1280, 720, "Wonky!");
}

void gameworld_start_loop(gameworld_t* world) {
#if defined(PLATFORM_WEB)
	emscripten_set_main_loop_arg(
		gameworld_start_emscripten_loop, world, 0, 1);
	return;
#endif
	gameworld_start_native_loop(world);
}

void gameworld_start_native_loop(gameworld_t* world) {
	while(!world->quit) {
		gameworld_main_loop(world);
		world->quit |= WindowShouldClose();
	}
	gameworld_deinit(world);
}

#if defined(PLATFORM_WEB)
void gameworld_start_emscripten_loop(void* user_data) {
	gameworld_t* world = (gameworld_t*)user_data;
	gameworld_main_loop(world);
}
#endif

void gameworld_main_loop(gameworld_t* world) {
	struct timeval t1, t2;
	double elapsedTime;
	gettimeofday(&t1, NULL);

	world->lag += world->delta_time;
	gameworld_handle_input(world);
	//printf("Tickrate: %f\n", gameworld->tickrate);
	//printf("lag: %f\n", gameworld->lag);
	// This needs to handle the difference between fps and
	// tickrate for justpressed events
	// maybe do input handling in normal update, then reset justpressed
	// after fixed update

	while(world->lag >= world->tickrate) {
		world->lag -= gaworld->tickrate;
		// if(world->isgameworld) {
		gameworld_update(world, world->tickrate);
		//input_reset(&world->input);
	}
	gameworld_render(world);

	gettimeofday(&t2, NULL);
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
	world->delta_time = elapsedTime / 1000;
}

void gameworld_handle_input(gameworld_t* world) {
	ecs_update_system(world->ecs, sys_input_handle, 0.f);
	//ecs_update_system(world->ecs, sys_input_move, 0.f);
}

void gameworld_update(gameworld_t* world, float dt) {
	ecs_update_system(world->ecs, sys_move_units, dt);
	ecs_update_system(world->ecs, sys_net_send_move, dt);
}

void gameworld_render(gameworld_t* world) {
	BeginDrawing();
	ClearBackground(RAYWHITE);
	ecs_update_system(world->ecs, sys_render_sprites, 0.f);
	ecs_update_system(world->ecs, sys_render_boxes, 0.f);
	EndDrawing();
	//SDL_SetRenderDrawColor(world->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderClear(world->renderer);
	//render_sprites(world);
	//SDL_RenderPresent(world->renderer);
}

int gameworld_deinit(gameworld_t* world) {
	//SDL_DestroyWindow(gameworld->window);
	//SDL_Quit();
	ecs_free(world->ecs);
	CloseWindow();
	return EXIT_SUCCESS;
}
