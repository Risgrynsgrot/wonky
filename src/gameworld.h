#pragma once
#include "inputsystem.h"
//#include "pico_ecs.h"
#include "map.h"
#include "traits.h"
#include "entity.h"

typedef struct gameworld {
	float tickrate;
	float lag;
	//input_t input;
    //graphics_t graphics;
	//ecs_t* ecs;

	trait_havers_t traits;
	entities_t entities;

    bool quit;
	input_map_t input_map[8];
	map_t map;
	bool headless;
	float delta_time;
	serializer_t net_writer;
} gameworld_t;

int gameworld_init(gameworld_t* world, bool headless);
void gameworld_start_loop(gameworld_t* world);
void gameworld_start_native_loop(gameworld_t* world);
void gameworld_start_emscripten_loop(void* user_data);
void gameworld_main_loop(gameworld_t* world);
void gameworld_raylib_init(void);
void gameworld_handle_input(gameworld_t* world);
void gameworld_update(gameworld_t* world, float dt);
void gameworld_render(gameworld_t* world);
int gameworld_deinit(gameworld_t* world);

//float client_get_deltatime(struct timespec before, struct timespec after);
