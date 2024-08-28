#include "client.h"
#include <stdio.h>

static bool keep_running = true;

bool client_init(client_t* client) {
	client->act.sa_handler = client_int_handler;
	sigaction(SIGINT, &client->act, NULL);
	sigaction(SIGQUIT, &client->act, NULL);

	client->host = enet_host_create(NULL, 1, 2, 0, 0);

	if(client->host == NULL) {
		fprintf(
			stderr,
			"An error occurred while trying to create an ENet client host.\n");
		return false;
	}

	enet_address_set_host(&client->address, "localhost");
	client->address.port = 1234; //set this in config

	client->peer = enet_host_connect(client->host, &client->address, 2, 0);
	if(client->peer == NULL) {
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
		enet_peer_reset(client->peer);
		printf(
			"Connection to localhost:1234 failed\n"); //change to real address
	}

	return true;
}

void client_update(client_t* client) {
	client->quit = !keep_running;
	while(!client->quit) {
		ENetEvent event;
		//poll for events
		//do update loop
		while(enet_host_service(client->host, &event, 1000) >
			  0) { //TODO(risgrynsgrot) the 1000 should be 0 for non blocking
			switch(event.type) {
			case ENET_EVENT_TYPE_CONNECT: //not sure this should be here on the
										  //client
				printf("client connected: %x:%u\n",
					   event.peer->address.host,
					   event.peer->address.port);
				event.peer->data = "Client information"; //Put client info here
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				printf(
					"A packet of length %zu containing %s was received from %s "
					"on channel %u\n",
					event.packet->dataLength,
					event.packet->data,
					(char*)event.peer->data,
					event.channelID);
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT: //not sure this should be here on
											 //the client
				printf("%s disconnected\n", (char*)event.peer->data);
				event.peer->data = NULL;
				break;
			default:
				break;
			}
		}
		//send results
	}
}

void client_deinit(client_t* client) {
	enet_host_destroy(client->host);
}

void client_int_handler(int value) {
	(void)value;
	keep_running = false;
}