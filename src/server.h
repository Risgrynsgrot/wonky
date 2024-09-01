#pragma once
#include <enet/enet.h>
#include <stdbool.h>
#include <signal.h>

typedef struct server {
	ENetHost* host;
	ENetAddress address;
	bool quit;

	struct sigaction act;
}
server_t;

bool server_init(server_t* server);
void server_update(server_t* server);
void server_deinit(server_t* server);
void server_int_handler(int value);

void server_send_broadcast(server_t* server, const char* data);
