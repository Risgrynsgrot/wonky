#pragma once
#include "serializer.h"
#include <enet/enet.h>

typedef struct net_string {
	const char* str;
	int length;
} net_string_t;

void net_peer_send(ENetPeer* peer, ser_net_t* ser);

void net_peer_receive(ENetPacket* packet);

void net_buffer_flush(net_buf_t* buf);
void net_buffer_print(net_buf_t* buf);
void net_buffer_reset(net_buf_t* buf);

void net_read_int(ser_net_t* ser, int32_t* value, const char* name);
void net_write_int(ser_net_t* ser, int32_t value, const char* name);

void net_read_bool(ser_net_t* ser, bool* value, const char* name);
void net_write_bool(ser_net_t* ser, bool value, const char* name);

void net_read_byte(ser_net_t* ser, char* value, const char* name);
void net_write_byte(ser_net_t* ser, char value, const char* name);

void net_read_ubyte(ser_net_t* ser, unsigned char* value, const char* name);
void net_write_ubyte(ser_net_t* ser, unsigned char value, const char* name);

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
