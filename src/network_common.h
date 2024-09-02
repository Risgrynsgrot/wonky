#pragma once
#include "serializer.h"
#include <enet/enet.h>

void net_send_peer(ENetPeer* peer, const char* data);

void net_read_int(ser_net_t* ser, int32_t* value, const char* name);
void net_write_int(ser_net_t* ser, int32_t value, const char* name);

void net_read_bool(ser_net_t* ser, bool* value, const char* name);
void net_write_bool(ser_net_t* ser, bool value, const char* name);

void net_read_float(ser_net_t* ser, float* value, const char* name);
void net_write_float(ser_net_t* ser, float value, const char* name);

void net_read_vector2(ser_net_t* ser, Vector2* value, const char* name);
void net_write_vector2(ser_net_t* ser, Vector2 value, const char* name);
