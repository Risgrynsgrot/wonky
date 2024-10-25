#include "server.h"
#include "components.h"
#include "net_data.h"
#include "network_common.h"
#include <stdio.h>
#include <string.h>
//#include <notcurses/direct.h>

static bool quit = false;

bool server_init(server_t* server) {

	//struct notcurses* terminal = notcurses_init(NULL, NULL);

	server->act.sa_handler = server_int_handler;
	sigaction(SIGINT, &server->act, NULL);
	sigaction(SIGQUIT, &server->act, NULL);

	//TODO(risgrynsgrot)
	//load config from lua file
	//setup enet server
	server->address.host = ENET_HOST_ANY;
	server->address.port = 1234;
	server->host = enet_host_create(&server->address, MAX_PLAYERS, 2, 0, 0);

	if(server->host == NULL) {
		fprintf(
			stderr,
			"An error occured while trying to create an ENet server host.\n");
		return false;
	}

	return true;
}

void server_update(server_t* server) {
	while(!quit) {
		ENetEvent event;
		//poll for events
		//do update loop
		while(enet_host_service(server->host, &event, 0) > 0) {
			switch(event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("client connected: %x:%u\n",
					   event.peer->address.host,
					   event.peer->address.port);

				int32_t player_id = net_player_add(&server->players);

				event.peer->data = &server->players.players[player_id];

				//server_send_broadcast(server, "nya");
				net_test_t test = {
					.a = 30, .extra = true, .b = 25, .c = 12, .d = 30};
				serializer_t ser = new_writer_network((ser_net_t){0});
				net_write_byte(&ser.ser.net, NET_TEST, "type");
				ser_net_test(&ser, &test);
				net_buffer_flush(&ser.ser.net.net_buf);
				net_buffer_print(&ser.ser.net.net_buf);
				net_peer_send(event.peer, &ser.ser.net);

				net_spawn_entity_t spawn_player = {
					.controller	 = player_id,
					.entity_type = {"player", strlen("player")},
					.position	 = {		0,				   0}
				 };


				break;
			case ENET_EVENT_TYPE_RECEIVE:
				net_peer_receive(event.packet);
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT: {
				net_player_t* player = event.peer->data;
				net_player_remove(&server->players, player->id);
				printf("%s disconnected\n", (char*)event.peer->data);
				event.peer->data = NULL;
				break;
			}
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
	quit = true;
}

void server_send_broadcast(server_t* server, const char* data) {
	ENetPacket* packet =
		enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->host, 0, packet);
}
