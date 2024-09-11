#include "network_common.h"
#include "components.h"
#include "raylib.h"
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

void net_peer_send(ENetPeer* peer, ser_net_t* ser) {
	ENetPacket* packet = enet_packet_create(ser->net_buf.data,
											ser->net_buf.word_index * 4,
											ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void net_peer_receive(ENetPacket* packet) {
	serializer_t ser = new_reader_network((ser_net_t){0});
	printf("data size: %lu", packet->dataLength);
	memcpy(ser.ser.net.net_buf.data, packet->data, packet->dataLength);
	net_buffer_print(&ser.ser.net.net_buf);

	char type;
	net_read_byte(&ser.ser.net, &type, "type");
	component_types_e comp_type = type;
	switch(comp_type) {
	case COMPONENT_NET_TEST:
		printf("received net test, serializing\n");
		comp_net_test_t result;
		ser_net_test(&ser, &result);
		printf("net_test values: %d, %d, %d, %d\n",
			   result.a,
			   result.b,
			   result.c,
			   result.d);
		break;
	default:
		printf("reading comp that doesn't exist: %d\n", comp_type);
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
	//add value to scratch, shifted by how much is in the scratch right now
	buf->scratch |= value << buf->scratch_bits;
	//shift how much to shift next time
	buf->scratch_bits += size_bits;
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

void net_read_bool(ser_net_t* ser, bool* value, const char* name) {
	(void)name;
	*value = net_bits_read(&ser->net_buf, 1);
}

void net_write_bool(ser_net_t* ser, bool value, const char* name) {
	(void)name;
	net_bits_write(&ser->net_buf, 1, value);
}

void net_read_byte(ser_net_t* ser, char* value, const char* name) {
	(void)name;
	*value = net_bits_read(&ser->net_buf, 8);
}

void net_write_byte(ser_net_t* ser, char value, const char* name) {
	(void)name;
	net_bits_write(&ser->net_buf, 8, value);
}

void net_read_ubyte(ser_net_t* ser, unsigned char* value, const char* name) {
	assert(false);
	(void)ser;
	(void)name;
	(void)value;
}

void net_write_ubyte(ser_net_t* ser, unsigned char value, const char* name) {
	assert(false);
	(void)ser;
	(void)name;
	(void)value;
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
	assert(false);
	(void)name;
	(void)ser;
	(void)value;
}

void net_write_string(ser_net_t* ser, net_string_t* value, const char* name) {
	assert(false);
	(void)name;
	(void)ser;
	(void)value;
}
