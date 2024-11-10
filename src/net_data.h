#pragma once
#include "network_common.h"
#include "serializer.h"
#include <raylib.h>
#include <stdint.h>

#define NET_DATA_ITER(_F, ...)                                                 \
	_F(test, TEST, 0, __VA_ARGS__)                                             \
	_F(move, MOVE, 1, __VA_ARGS__)                                             \
	_F(spawn_entity, SPAWN_ENTITY, 2, __VA_ARGS__)                             \
	_F(entity_state, ENTITY_STATE, 3, __VA_ARGS__)

#define DECL_ENUM_NET_DATA(lc, uc, i, ...) NET_##uc = i,

typedef enum net_types { NET_DATA_ITER(DECL_ENUM_NET_DATA, void) } net_types_e;

typedef struct net_test {
	int32_t a;
	bool extra;
	int32_t b;
	int32_t c;
	int32_t d;
} net_test_t;

void ser_net_test(serializer_t* ser, void* data);

typedef struct net_move { //move this to separate net structs
	Vector2 from_tile;
	Vector2 to_tile;
	int32_t entity_id;
} net_move_t;

void ser_net_move(serializer_t* ser, void* data);
void net_handle_move(gameworld_t* world,
					 net_move_t* data,
					 net_player_t* player);

typedef struct net_spawn_entity {
	net_string_t entity_type;
	int32_t controller; //who owns the entity
	Vector2 position;
} net_spawn_entity_t;

void ser_spawn_entity(serializer_t* ser, void* data);
void net_handle_spawn_entity(gameworld_t* world, net_spawn_entity_t* data);

typedef struct net_entity_state {
	entity_t entity;
	component_types_e components;
	uint8_t* data;
} net_entity_state_t;

void ser_entity_state(serializer_t* ser, void* data);
void net_handle_entity_state(gameworld_t* world, net_entity_state_t* data);
