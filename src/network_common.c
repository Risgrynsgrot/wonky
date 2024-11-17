#include "network_common.h"
#include "components.h"
#include "gameworld.h"
#include "net_data.h"
#include "raylib.h"
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int32_t net_player_add(net_players_t* players) {
	int32_t result = -1;
	if(players->count >= MAX_PLAYERS) {
		printf("tried to add player, but players were full\n");
		return result;
	}
	for(int i = 0; i < MAX_PLAYERS; i++) {
		net_player_t* player = &players->players[i];
		if(!player->active) {
			player->active = true;
			players->count++;
			result = i;
			return result;
		}
	}
	assert(false && "somehow no players were inactive even though there "
					"should be space");
	return -1;
}

void net_player_remove(net_players_t* players, int32_t id) {
	if(id >= MAX_PLAYERS) {
		printf("tried to remove player that is impossible to exist");
		return;
	}
	players->players[id].active = false;
	players->count--;
}

void net_peer_send(ENetPeer* peer, ser_net_t* ser) {
	if(ser->net_buf.word_index <= 0) {
		return;
	}
	//printf("packet size to send: %d\n", ser->net_buf.word_index);
	ENetPacket* packet = enet_packet_create(ser->net_buf.data,
											ser->net_buf.word_index * 4,
											ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void net_peer_receive(gameworld_t* world, ENetPacket* packet, ENetPeer* peer) {
	serializer_t ser = new_reader_network((ser_net_t){0});
	printf("data size: %lu", packet->dataLength);
	memcpy(ser.ser.net.net_buf.data, packet->data, packet->dataLength);
	net_buffer_print(&ser.ser.net.net_buf);

	int8_t type;
	net_read_byte(&ser.ser.net, &type, "type");
	net_types_e net_type = type;
	switch(net_type) {
	case NET_TEST:
		printf("received net test, serializing\n");
		net_test_t result;
		ser_net_test(&ser, &result);
		printf("net_test values: %d, %d, %d, %d\n",
			   result.a,
			   result.b,
			   result.c,
			   result.d);
		break;
	case NET_MOVE: {
		printf("received net move, serializing\n");
		net_move_t net_move;
		ser_net_move(&ser, &net_move);

		net_handle_move(world, &net_move, peer->data);
		printf("net_move values:\nfrom: %f, %f\nto: %f, %f\nentity: %u",
			   net_move.from_tile.x,
			   net_move.from_tile.y,
			   net_move.to_tile.x,
			   net_move.to_tile.y,
			   net_move.entity_id);
		break;
	}
	case NET_SPAWN_ENTITY: {
		printf("received net spawn, serializing\n");
		net_spawn_entity_t net_spawn_entity;
		ser_spawn_entity(&ser, &net_spawn_entity);
		net_handle_spawn_entity(world, &net_spawn_entity);
		break;
	}
	default:
		printf("reading netdata that doesn't exist: %d\n", net_type);
		break;
	}
}

void net_buffer_reset(net_buf_t* buf) {
	buf->word_index	   = 0;
	buf->scratch	   = 0;
	buf->scratch_bits  = 0;
	buf->num_bits_read = 0;
}

void net_buffer_flush(net_buf_t* buf) {
	if(buf->scratch_bits == 0) {
		return;
	}
	buf->total_bits += buf->scratch_bits;
	buf->data[buf->word_index] = (buf->scratch & 0xFFFFFFFF);
	buf->scratch_bits		   = 0;
	buf->scratch >>= 32;
	buf->word_index++;
}

void net_buffer_print(net_buf_t* buf) {
	printf("buffer contains: ");
	for(int i = 0; i < NET_MAX_PACKET_SIZE; i++) {
		printf("%#08x,", buf->data[i]);
	}
	printf("\n");
}

void net_bits_write(net_buf_t* buf, int32_t size_bits, int32_t value) {
	assert(size_bits + buf->total_bits <=
		   NET_MAX_PACKET_SIZE * sizeof(uint32_t));
	//add value to scratch, shifted by how much is in the scratch right now
	buf->scratch |= value << buf->scratch_bits;
	//shift how much to shift next time
	buf->scratch_bits += size_bits;
	buf->total_bits += size_bits;
	//if scratch is full
	if(buf->scratch_bits >= 32) {
		//move from scratch to buffer
		buf->data[buf->word_index] = buf->scratch & 0xFFFFFFFF;
		buf->word_index++;		 //move buffer index
		buf->scratch >>= 32;	 //shift scratch if there is anything left in it
		buf->scratch_bits -= 32; //and move the index back
	}
}

uint32_t net_bits_read(net_buf_t* buf, int32_t size_bits) {

	if(buf->scratch_bits < size_bits) {
		buf->scratch |= (uint64_t)buf->data[buf->word_index]
						<< buf->scratch_bits;
		buf->scratch_bits += 32;
		buf->word_index++;
	}
	int32_t result =
		buf->scratch & (((uint64_t)1 << (uint64_t)size_bits) - (uint64_t)1);
	buf->scratch >>= size_bits;
	buf->scratch_bits -= size_bits;
	buf->num_bits_read += size_bits;

	return result;
}

void net_read_int(ser_net_t* ser, int32_t* value, const char* name) {
	(void)name;
	*value = net_bits_read(&ser->net_buf, 32);
}

void net_write_int(ser_net_t* ser, int32_t value, const char* name) {
	(void)name;
	net_bits_write(&ser->net_buf, 32, value);
}

void net_read_uint(ser_net_t* ser, uint32_t* value, const char* name) {
	(void)name;
	*value = net_bits_read(&ser->net_buf, 32);
}

void net_write_uint(ser_net_t* ser, uint32_t value, const char* name) {
	(void)name;
	net_bits_write(&ser->net_buf, 32, value);
}

void net_read_bool(ser_net_t* ser, bool* value, const char* name) {
	(void)name;
	*value = net_bits_read(&ser->net_buf, 1);
}

void net_write_bool(ser_net_t* ser, bool value, const char* name) {
	(void)name;
	net_bits_write(&ser->net_buf, 1, value);
}

void net_read_byte(ser_net_t* ser, int8_t* value, const char* name) {
	(void)name;
	*value = net_bits_read(&ser->net_buf, 8);
}

void net_write_byte(ser_net_t* ser, int8_t value, const char* name) {
	(void)name;
	net_bits_write(&ser->net_buf, 8, value);
}

void net_read_ubyte(ser_net_t* ser, uint8_t* value, const char* name) {
	(void)name;
	*value = net_bits_read(&ser->net_buf, 8);
}

void net_write_ubyte(ser_net_t* ser, uint8_t value, const char* name) {
	(void)name;
	net_bits_write(&ser->net_buf, 8, value);
}

void net_read_float(ser_net_t* ser, float* value, const char* name) {
	(void)name;
	*value = net_bits_read(&ser->net_buf, 32);
}

void net_write_float(ser_net_t* ser, float value, const char* name) {
	(void)name;
	net_bits_write(&ser->net_buf, 32, value);
}

void net_read_double(ser_net_t* ser, double* value, const char* name) {
	assert(false);
	(void)name;
	(void)ser;
	(void)value;
}

void net_write_double(ser_net_t* ser, double value, const char* name) {
	assert(false);
	(void)name;
	(void)ser;
	(void)value;
}

void net_read_vector2(ser_net_t* ser, Vector2* value, const char* name) {
	net_read_float(ser, &value->x, name);
	net_read_float(ser, &value->y, name);
}

void net_write_vector2(ser_net_t* ser, Vector2 value, const char* name) {
	net_write_float(ser, value.x, name);
	net_write_float(ser, value.y, name);
}

void net_read_color(ser_net_t* ser, Color* value, const char* name) {
	net_read_ubyte(ser, &value->r, name);
	net_read_ubyte(ser, &value->g, name);
	net_read_ubyte(ser, &value->b, name);
	net_read_ubyte(ser, &value->a, name);
}

void net_write_color(ser_net_t* ser, Color value, const char* name) {
	net_write_ubyte(ser, value.r, name);
	net_write_ubyte(ser, value.g, name);
	net_write_ubyte(ser, value.b, name);
	net_write_ubyte(ser, value.a, name);
}

void net_read_string(ser_net_t* ser, net_string_t* value, const char* name) {
	(void)name;
	assert(value->length <= MAX_NET_STRING_LENGTH);
	net_read_uint(ser, &value->length, "length");
	for(uint32_t i = 0; i < value->length; i++) {
		printf("current char: %c\n", value->str[i]);
		net_read_byte(ser, &value->str[i], "");
	}
}

void net_write_string(ser_net_t* ser, net_string_t* value, const char* name) {
	(void)name;
	assert(value->length <= MAX_NET_STRING_LENGTH);
	printf("length: %i", value->length);
	net_write_uint(ser, value->length, "length");
	for(uint32_t i = 0; i < value->length; i++) {
		printf("current char: %c\n", value->str[i]);
		net_write_byte(ser, value->str[i], "");
	}
}

void net_read_entity(ser_net_t* ser, entity_t* value, const char* name) {
	(void)name;
	net_read_int(ser, &value->id, "id");
	net_read_uint(ser, &value->generation, "generation");
}

void net_write_entity(ser_net_t* ser, entity_t value, const char* name) {
	(void)name;
	net_write_int(ser, value.id, "id");
	net_write_uint(ser, value.generation, "generation");
}
