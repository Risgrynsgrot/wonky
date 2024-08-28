#include "server.h"
#include <stdio.h>

static bool keep_running = true;

bool server_init(server_t* server) {

	server->act.sa_handler = server_int_handler;
	sigaction(SIGINT, &server->act, NULL);
	sigaction(SIGQUIT, &server->act, NULL);

	//TODO(risgrynsgrot)
	//load config from lua file
	//setup enet server
	server->address.host = ENET_HOST_ANY;
	server->address.port = 1234;
	server->host		 = enet_host_create(&server->address, 32, 2, 0, 0);
	server->quit		 = false;

	if(server->host == NULL) {
		fprintf(
			stderr,
			"An error occured while trying to create an ENet server host.\n");
		return false;
	}

	return true;
}

void server_update(server_t* server) {
	server->quit = !keep_running;
	while(!server->quit) {
		ENetEvent event;
		//poll for events
		//do update loop
		while(enet_host_service(server->host, &event, 1000) >
			  0) { //TODO(risgrynsgrot) the 1000 should be 0 for non blocking
			switch(event.type) {
			case ENET_EVENT_TYPE_CONNECT:
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
			case ENET_EVENT_TYPE_DISCONNECT:
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

void server_deinit(server_t* server) {
	enet_host_destroy(server->host);
}

void server_int_handler(int value) {
	(void)value;
	keep_running = false;
}
