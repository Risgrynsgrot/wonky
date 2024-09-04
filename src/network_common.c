#include "network_common.h"
#include "raylib.h"
#include <arpa/inet.h>
#include <string.h>

void net_send_peer(ENetPeer* peer, const char* data) {
	ENetPacket* packet =
		enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void* net_buffer_read(net_buf_t* buf, int32_t size) {
	void* result = &buf->buffer[buf->index];
	buf->index += size;
	return result;
}

void net_buffer_write(net_buf_t* buf, int32_t size, void* value) {
	for(int i = 0; i < size; i++) {
		buf->buffer[buf->index + i] =
			*((char*)(value) + i); //this needs to be checked that it's valid
	}
	buf->index += size;
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
