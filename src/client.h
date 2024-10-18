#pragma once
#include "gameworld.h"
#include <enet/enet.h>
#include <signal.h>
#include <stdbool.h>

typedef struct client {
	gameworld_t gameworld;

	ENetHost* host;
	ENetPeer* server;
	ENetAddress address;
	struct sigaction act;
} client_t;

bool client_init(client_t* client);
void client_update(client_t* client);
void client_deinit(client_t* client);
void client_int_handler(int value);
