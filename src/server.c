#include "server.h"
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

	gameworld_init(&server->world, true);

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

				int32_t player_id = net_player_add(&server->world.players);

				event.peer->data = &server->world.players.players[player_id];

				//server_send_broadcast(server, "nya");
				serializer_t ser =
					new_writer_network((ser_net_t){0}, &server->world);
				net_write_byte(&ser.ser.net, NET_SPAWN_ENTITY, "type");
				net_spawn_entity_t spawn_player = {
					.entity_type = { "player", strlen("player")},
					.controller	 = player_id,
					.position	 = {player_id,				   0}
				 };
				net_handle_spawn_entity(&server->world, &spawn_player);
				ser_spawn_entity(&ser, &spawn_player);
				net_buffer_flush(&ser.ser.net.net_buf);
				net_buffer_print(&ser.ser.net.net_buf);
				net_peer_send(event.peer, &ser.ser.net);

				break;
			case ENET_EVENT_TYPE_RECEIVE:
				net_peer_receive(&server->world, event.packet, event.peer);
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT: {
				net_player_t* player = event.peer->data;
				net_player_remove(&server->world.players, player->id);
				printf("%s disconnected\n", (char*)event.peer->data);
				event.peer->data = NULL;
				break;
			}
			default:
				break;
			}
		}

		gameworld_main_loop(&server->world);
		server_send_broadcast(server, &server->world.net_writer.ser.net);
		//full buffer reset because it's sent
		server->world.net_writer.ser.net.net_buf = (net_buf_t){0};
	}
}

void server_deinit(server_t* server) {
	enet_host_destroy(server->host);
}

void server_int_handler(int value) {
	(void)value;
	quit = true;
}

void server_send_broadcast(server_t* server, ser_net_t* ser) {
	if(ser->net_buf.word_index <= 0) {
		return;
	}
	ENetPacket* packet = enet_packet_create(ser->net_buf.data,
											ser->net_buf.word_index * 4,
											ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server->host, 0, packet);
}
