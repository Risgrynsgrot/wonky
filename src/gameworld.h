#pragma once
#include "inputsystem.h"
#include "pico_ecs.h"
#include "map.h"

typedef struct gameworld {
	float tickrate;
	float lag;
	//input_t input;
    //graphics_t graphics;
	ecs_t* ecs;
    bool quit;
	input_map_t input_map[8];
	map_t map;
} gameworld_t;

int gameworld_init(gameworld_t* client);
void gameworld_start_loop(gameworld_t* client);
void gameworld_start_native_loop(gameworld_t* client);
void gameworld_start_emscripten_loop(void* user_data);
void gameworld_main_loop(gameworld_t* client);
void gameworld_raylib_init(void);
void gameworld_handle_input(gameworld_t* client);
void gameworld_update(gameworld_t* client, float dt);
void gameworld_render(gameworld_t* client);
int gameworld_deinit(gameworld_t* client);

//float client_get_deltatime(struct timespec before, struct timespec after);
