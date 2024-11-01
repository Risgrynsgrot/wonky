#include "client.h"
#include "network_common.h"
#include <stdio.h>

static bool quit = false;

bool client_init(client_t* client) {
	client->act.sa_handler = client_int_handler;
	sigaction(SIGINT, &client->act, NULL);
	sigaction(SIGQUIT, &client->act, NULL);

	gameworld_init(&client->world, false);

	client->host = enet_host_create(NULL, 1, 2, 0, 0);

	if(client->host == NULL) {
		fprintf(
			stderr,
			"An error occurred while trying to create an ENet client host.\n");
		return false;
	}

	enet_address_set_host(&client->address, "localhost");
	client->address.port = 1234; //set this in config

	client->server = enet_host_connect(client->host, &client->address, 2, 0);
	if(client->server == NULL) {
		fprintf(stderr,
				"No available peers for initiating an ENet connection\n");
		return false;
	}

	ENetEvent event;
	if(enet_host_service(client->host, &event, 5000) > 0 &&
	   event.type == ENET_EVENT_TYPE_CONNECT) {
		printf("Connection to localhost:1234 succeeded\n"); //change to real
															//address
	} else {
		enet_peer_reset(client->server);
		printf(
			"Connection to localhost:1234 failed\n"); //change to real address
	}

	//net_peer_send(client->server, "bruh");

	return true;
}

void client_update(client_t* client) {
	while(!quit) {
		ENetEvent event;
		while(enet_host_service(client->host, &event, 0) > 0) {
			switch(event.type) {
			case ENET_EVENT_TYPE_RECEIVE:
				net_peer_receive(&client->world, event.packet);
				enet_packet_destroy(event.packet);
				break;
			default:
				break;
			}
		}

		//poll for events
		gameworld_main_loop(&client->world);
		net_peer_send(client->server, &client->world.net_writer.ser.net);
		//full buffer reset because it's sent
		client->world.net_writer.ser.net.net_buf = (net_buf_t){0};
	}
}

void client_deinit(client_t* client) {
	gameworld_deinit(&client->world);
	enet_host_destroy(client->host);
}

void client_int_handler(int value) {
	(void)value;
	printf("killing client\n");
	quit = true;
}
