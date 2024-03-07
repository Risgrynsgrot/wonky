#pragma once
#include "raylib.h"

typedef struct client client_t;

typedef struct input_map {
	KeyboardKey left;
	KeyboardKey right;
	KeyboardKey up;
	KeyboardKey down;
	KeyboardKey interact;
	KeyboardKey open_inventory;
} input_map_t;

input_map_t input_init(void);
void input_handle(client_t* client);
void input_move(client_t* client);
