#pragma once
#include "raylib.h"
#include "pico_ecs.h"
#include "systems.h"

typedef struct input_map {
	KeyboardKey left;
	KeyboardKey right;
	KeyboardKey up;
	KeyboardKey down;
	KeyboardKey interact;
	KeyboardKey open_inventory;
} input_map_t;

input_map_t input_init(void);
void ecs_register_input_systems(ecs_t* ecs, input_map_t* keymap);
ECS_DECL_SYSTEM(input_handle);
//ECS_DECL_SYSTEM(input_move);
