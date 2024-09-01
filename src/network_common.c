#include "network_common.h"
#include <string.h>

void net_send_peer(ENetPeer* peer, const char* data) {
	ENetPacket* packet =
		enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}

void net_read_int(ser_network_t* ser, int* value, const char* name) {

}
