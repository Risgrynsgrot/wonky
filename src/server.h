#pragma once
#include "network_common.h"
#include <enet/enet.h>
#include <stdbool.h>
#include <signal.h>


typedef struct server {
	ENetHost* host;
	ENetAddress address;

	net_players_t players;

	struct sigaction act;
}
server_t;

bool server_init(server_t* server);
void server_update(server_t* server);
void server_deinit(server_t* server);
void server_int_handler(int value);

void server_send_broadcast(server_t* server, const char* data);

void server_spawn_player(server_t* server, ENetEvent* event);
