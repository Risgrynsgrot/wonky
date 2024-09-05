#include "network_common.h"
#include "raylib.h"
#include <arpa/inet.h>
#include <string.h>

void net_send_peer(ENetPeer* peer, const char* data) {
	ENetPacket* packet =
		enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void net_buffer_reset(net_buf_t* buf) {
	buf->word_index	  = 0;
	buf->scratch	  = 0;
	buf->scratch_bits = 0;
}

void net_buffer_flush(net_buf_t* buf) {
	if(buf->scratch_bits == 0) {
		return;
	}
	buf->buffer[buf->word_index] = (buf->scratch & 0xFFFFFFFF);
	buf->scratch_bits			 = 0;
	buf->scratch >>= 32;
	buf->word_index++;
}

void net_bits_write(net_buf_t* buf, int32_t size_bits, int32_t value) {
	buf->scratch |=
		value << buf->scratch_bits; //add value to scratch, shifted by how much
									//is in the scratch right now
	buf->scratch_bits += size_bits; //shift how much to shift next time
	if(buf->scratch_bits >= 32) {	//if scratch is full
		buf->buffer[buf->word_index] =
			buf->scratch & 0xFFFFFFFF; //move from scratch to buffer
		buf->word_index++;			   //move buffer index
		buf->scratch >>= 32;	 //shift scratch if there is anything left in it
		buf->scratch_bits -= 32; //and move the index back
	}
}

int32_t net_bits_read(net_buf_t* buf, int32_t size_bits) {
	buf->scratch = buf->buffer[buf->word_index] << buf->scratch_bits;
}

void net_read_int(ser_net_t* ser, int32_t* value, const char* name) {
	(void)name;
	int32_t* net_value = net_buffer_read(&ser->net_buf, sizeof(int32_t));
	*value			   = ntohl(*net_value);
}

void net_write_int(ser_net_t* ser, int32_t value, const char* name) {
	(void)name;
	int32_t net_value = htonl(value);
	net_buffer_write(&ser->net_buf, sizeof(net_value), &net_value);
}

void net_read_bool(ser_net_t* ser, bool* value, const char* name) {
	(void)ser;
	(void)name;
	(void)value;
}

void net_write_bool(ser_net_t* ser, bool value, const char* name) {
	(void)ser;
	(void)name;
	(void)value;
}

void net_read_byte(ser_net_t* ser, char* value, const char* name) {
	(void)ser;
	(void)name;
	(void)value;
}

void net_write_byte(ser_net_t* ser, char value, const char* name) {
	(void)ser;
	(void)name;
	(void)value;
}

void net_read_ubyte(ser_net_t* ser, unsigned char* value, const char* name) {
	(void)ser;
	(void)name;
	(void)value;
}

void net_write_ubyte(ser_net_t* ser, unsigned char value, const char* name) {
	(void)ser;
	(void)name;
	(void)value;
}

void net_read_float(ser_net_t* ser, float* value, const char* name) {
	(void)name;
	float* net_value = net_buffer_read(&ser->net_buf, sizeof(float));
	*value			 = ntohl(*net_value);
}

void net_write_float(ser_net_t* ser, float value, const char* name) {
	(void)name;
	float net_value = htonl(value);
	net_buffer_write(&ser->net_buf, sizeof(net_value), &net_value);
}

void net_read_double(ser_net_t* ser, double* value, const char* name) {
	(void)name;
	double* net_value = net_buffer_read(&ser->net_buf, sizeof(double));
	*value			  = ntohl(*net_value);
}

void net_write_double(ser_net_t* ser, double value, const char* name) {
	(void)name;
	double net_value = htonl(value);
	net_buffer_write(&ser->net_buf, sizeof(net_value), &net_value);
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
}

void net_write_string(ser_net_t* ser, net_string_t* value, const char* name) {
}
