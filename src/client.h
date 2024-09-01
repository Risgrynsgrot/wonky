#pragma once
#include <enet/enet.h>
#include <stdbool.h>
#include <signal.h>

typedef struct client {
	ENetHost* host;
	ENetPeer* server;
	ENetAddress address;
	bool quit;

	struct sigaction act;
} client_t;

bool client_init(client_t* client);
void client_update(client_t* client);
void client_deinit(client_t* client);
void client_int_handler(int value);
