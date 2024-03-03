#pragma once
#include "components.h"

typedef struct client {
	float tickrate;
	//input_t input;
    //graphics_t graphics;
    components_t* components;
    int empty_ids[MAX_ENTITIES];
    int empty_id_count;
    bool quit;
} client_t;

int client_init(client_t* client);
void client_start_loop(client_t* client);
void client_raylib_init(void);
//void client_handle_input(client_t* client);
void client_update(client_t* client, float dt);
void client_render(client_t* client);
int client_deinit(client_t* client);

//float client_get_deltatime(struct timespec before, struct timespec after);
