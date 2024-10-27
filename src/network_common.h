#pragma once
#include "serializer.h"
#include <enet/enet.h>

#define MAX_PLAYERS 32

typedef struct gameworld gameworld_t;

typedef enum net_controller {
	NET_CONTROLLER_GAME = -1,
} net_controller_e;

#define MAX_NET_STRING_LENGTH 512
typedef struct net_string {
	int8_t str[MAX_NET_STRING_LENGTH];
	uint32_t length;
} net_string_t;

typedef struct net_player {
	ENetPeer* peer;
	uint32_t id;
	//entity id of controlled character, so we can map between server and client
	uint32_t entity_id;
	bool active;
} net_player_t;

typedef struct net_players {
	net_player_t players[MAX_PLAYERS];
	int32_t count;
} net_players_t;

int32_t net_player_add(net_players_t* players);
void net_player_remove(net_players_t* players, int32_t id);

void net_peer_send(ENetPeer* peer, ser_net_t* ser);

void net_peer_receive(gameworld_t* world, ENetPacket* packet);

void net_buffer_flush(net_buf_t* buf);
void net_buffer_print(net_buf_t* buf);
void net_buffer_reset(net_buf_t* buf);

void net_read_int(ser_net_t* ser, int32_t* value, const char* name);
void net_write_int(ser_net_t* ser, int32_t value, const char* name);

void net_read_uint(ser_net_t* ser, uint32_t* value, const char* name);
void net_write_uint(ser_net_t* ser, uint32_t value, const char* name);

void net_read_bool(ser_net_t* ser, bool* value, const char* name);
void net_write_bool(ser_net_t* ser, bool value, const char* name);

void net_read_byte(ser_net_t* ser, int8_t* value, const char* name);
void net_write_byte(ser_net_t* ser, int8_t value, const char* name);

void net_read_ubyte(ser_net_t* ser, uint8_t* value, const char* name);
void net_write_ubyte(ser_net_t* ser, uint8_t value, const char* name);

void net_read_float(ser_net_t* ser, float* value, const char* name);
void net_write_float(ser_net_t* ser, float value, const char* name);

void net_read_double(ser_net_t* ser, double* value, const char* name);
void net_write_double(ser_net_t* ser, double value, const char* name);

void net_read_vector2(ser_net_t* ser, Vector2* value, const char* name);
void net_write_vector2(ser_net_t* ser, Vector2 value, const char* name);

void net_read_color(ser_net_t* ser, Color* value, const char* name);
void net_write_color(ser_net_t* ser, Color value, const char* name);

void net_read_string(ser_net_t* ser, net_string_t* value, const char* name);
void net_write_string(ser_net_t* ser, net_string_t* value, const char* name);
