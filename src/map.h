#pragma once
#include <raymath.h>
#include <stdbool.h>
#include <stdint.h>

#define JSON_GET(TYPE, ROOT, JSON_NAME, RESULT) \
do { \
	printf("l: %d, f: %s, loading:%s\n", __LINE__, __FILE__, #JSON_NAME);\
	json_object* JSON_NAME##_json;\
	bool result = json_object_object_get_ex(ROOT, #JSON_NAME, &JSON_NAME##_json);\
	printf("got value: %i\n", result);\
	if(!result) {\
		printf("%s doesn't exist, skipping\n", #JSON_NAME);\
		break;\
	}\
	if (json_object_get_type(JSON_NAME##_json) == json_type_null){ \
		printf("%s is null, skipping\n", #JSON_NAME);\
		break;\
	}\
	RESULT = json_object_get_##TYPE(JSON_NAME##_json);\
	printf("loaded %s.\n", #JSON_NAME);\
} while(0)

#define JSON_GET_STR(ROOT, JSON_NAME, RESULT) \
do { \
	printf("l: %d, f: %s, loadString:%s\n", __LINE__, __FILE__, #JSON_NAME);\
	json_object* JSON_NAME##_json;\
	bool result = json_object_object_get_ex(ROOT, #JSON_NAME, &JSON_NAME##_json);\
	printf("got value: %i\n", result);\
	if(!result){\
		printf("%s is null, skipping\n", #JSON_NAME);\
		break;\
	}\
	if (json_object_get_type(JSON_NAME##_json) == json_type_null){ \
		printf("%s is null, skipping\n", #JSON_NAME);\
		break;\
	}\
	const char* temp = json_object_get_string(JSON_NAME##_json);\
	strcpy(RESULT, temp);\
	printf("loaded %s: %s\n", #JSON_NAME, RESULT);\
} while(0)

typedef struct ldtk_level_neighbour {
	char dir[512];
	char level_iid[512];
} ldtk_level_neighbour_t;

typedef struct ldtk_grid_point {
	int32_t cx;
	int32_t cy;
} ldtk_grid_point_t;

typedef struct ldtk_entity_ref {
	char entity_iid[512];
	char layer_iid[512];
	char level_iid[512];
	char work_iid[512];
} ldtk_entity_ref_t;

typedef struct ldtk_ts_rect {
	int32_t w;
	int32_t h;
	int32_t x;
	int32_t y;
	int32_t tileset_uid;
} ldtk_ts_rect_t;

typedef struct ldtk_tile {
	float a;
	int f;
	Vector2 px;
	Vector2 src;
	int t;
} ldtk_tile_t;

typedef struct ldtk_field_value {
	union {
		int value_int;
		float value_float;
		bool value_bool;
		char value_string[512];
		ldtk_ts_rect_t value_tile;
		ldtk_grid_point_t value_point;
		ldtk_entity_ref_t value_entity_ref;
	};

} ldtk_field_value_t;

typedef struct ldtk_field {
	char identifier[512];
	ldtk_ts_rect_t tile;
	char type[512];
	ldtk_field_value_t value;
	int32_t def_uid;
} ldtk_field_t;

typedef struct ldtk_entity {
	Vector2 grid;
	char identifier[512];
	Vector2 pivot;
	char smart_color[512];
	char tags[512];
	int32_t tag_count;
	ldtk_ts_rect_t tile;
	int32_t world_x;
	int32_t world_y;
	int32_t def_uid;
	ldtk_field_t* fields;
	int32_t field_count;
	int32_t width;
	int32_t height;
	char iid[512];
	Vector2 px;
} ldtk_entity_t;

typedef struct ldtk_layer {
	int32_t c_hei;
	int32_t c_wid;
	int32_t grid_size;
	char identifier[512];
	float opacity;
	int32_t px_total_offset_x;
	int32_t px_total_offset_y;
	int32_t tileset_def_uid;
	char tileset_rel_path[512];
	char type[512];
	ldtk_tile_t* auto_layer_tiles;
	int32_t auto_layer_tile_count;
	ldtk_entity_t* entities;
	int32_t entity_count;
	ldtk_tile_t* grid_tiles;
	int32_t grid_tile_count;
	char iid[512];
	int32_t* int_grid_csv;
	int32_t int_grid_count;
	int32_t layer_def_uid;
	int32_t level_id;
	int32_t override_tileset_uid;
	int32_t px_offset_x;
	int32_t px_offset_y;
	bool visible;
} ldtk_layer_t;

typedef struct ldtk_level {
	char bg_rel_path[512];
	char external_rel_path[512];
	ldtk_field_t* fields;
	int32_t field_count;
	char identifier[512];
	char iid[512];
	ldtk_layer_t* layers;
	int32_t layer_count;
	int32_t px_hei;
	int32_t px_wid;
	int32_t uid;
	int32_t world_depth;
	int32_t world_x;
	int32_t world_y;
} ldtk_level_t;

//typedef struct ldtk_defs {
//	ldtk_layer_t* layers;
//	int layer_count;
//	ldtk_entity_t* entities;
//	int entity_count;
//	ldtk_tileset_t* tilesets;
//	int tileset_count;
//} ldtk_defs_t;

typedef struct ldtk_map {
	char bg_color[512];
	//defs
	bool external_levels;
	char iid[512];
	char json_version[512];
	ldtk_level_t* levels;
	int32_t level_count;
	//toc, seems to be some top level thing, add later if relevant
	int32_t world_grid_height;
	int32_t world_grid_width;
	char world_layout[512];
	//worlds
} ldtk_map_t;

bool map_load_ldtk(const char* path, ldtk_map_t* map);

typedef struct ecs_s ecs_t;
bool map_spawn_entities(ldtk_layer_t* layer, ecs_t* ecs);
