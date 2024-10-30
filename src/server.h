#pragma once
#include "gameworld.h"
#include "network_common.h"
#include <enet/enet.h>
#include <signal.h>
#include <stdbool.h>

typedef struct server {
	ENetHost* host;
	ENetAddress address;

	net_players_t players;
	gameworld_t world;

	struct sigaction act;
} server_t;

bool server_init(server_t* server);
void server_update(server_t* server);
void server_deinit(server_t* server);
void server_int_handler(int value);

void server_send_broadcast(server_t* server, const char* data);

void server_spawn_player(server_t* server, ENetEvent* event);
