//#include "gameworld.h"
#include "client.h"
#include "server.h"
#include <enet/enet.h>
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char** argv) {
	bool is_server = false;

	for(int i = 1; i < argc; i++) {
		const char* value = argv[i];
		if(value[0] != '-') {
			continue;
		}
		switch(value[1]) {
		case 's':
			is_server = true;
			break;
		}
	}

	if(enet_initialize() != 0) {
		fprintf(stderr, "An error occurred while initializing ENet\n");
	}

	if(is_server) {
		server_t server;
		server_init(&server);
		server_update(&server);
		server_deinit(&server);
	} else {
		client_t client;
		client_init(&client);
		client_update(&client);
		client_deinit(&client);
	}

	//gameworld_t gameworld;
	//gameworld_init(&gameworld);
	//gameworld_start_loop(&gameworld);

	enet_deinitialize();
}
