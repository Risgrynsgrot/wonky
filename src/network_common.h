#pragma once
#include "serializer.h"
#include <enet/enet.h>

void net_send_peer(ENetPeer* peer, const char* data);

void net_read_int(ser_network_t* ser, int* value, const char* name);
