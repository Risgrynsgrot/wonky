#pragma once
#include <raymath.h>
#include <stdbool.h>
#include <stdint.h>

#define JSON_ARR_LEN(ROOT, JSON_NAME, RESULT) \
do { \
	json_object* JSON_NAME##_json = json_object_object_get(ROOT, #JSON_NAME);\
	RESULT = json_object_array_length(JSON_NAME##_json);\
} while(0)

#define JSON_GET(TYPE, ROOT, JSON_NAME, RESULT) \
do { \
	json_object* JSON_NAME##_json = json_object_object_get(ROOT, #JSON_NAME);\
	RESULT = json_object_get_##TYPE(JSON_NAME##_json);\
} while(0)

typedef struct ldtk_level_neighbour {
	const char* dir;
	const char* level_iid;
} ldtk_level_neighbour_t;

typedef struct ldtk_grid_point {
	int32_t cx;
	int32_t cy;
} ldtk_grid_point_t;

typedef struct ldtk_entity_ref {
	const char* entity_iid;
	const char* layer_iid;
	const char* level_iid;
	const char* work_iid;
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
		const char* value_string; //If this causes issues, expand string
		ldtk_ts_rect_t value_tile;
		ldtk_grid_point_t value_point;
		ldtk_entity_ref_t value_entity_ref;
	};

} ldtk_field_value_t;

typedef struct ldtk_field {
	const char* identifier;
	ldtk_ts_rect_t tile;
	const char* type;
	ldtk_field_value_t value;
	int32_t def_uid;
} ldtk_field_t;

typedef struct ldtk_entity {
	Vector2 grid;
	const char* identifier;
	Vector2 pivot;
	const char* smart_color;
	const char** tags;
	int32_t tag_count;
	ldtk_ts_rect_t tile;
	int32_t world_x;
	int32_t world_y;
	int32_t def_uid;
	ldtk_field_t* field_instances;
	int32_t field_instance_count;
	int32_t width;
	int32_t height;
	const char* iid;
	Vector2 px;
} ldtk_entity_t;

typedef struct ldtk_layer {
	int32_t c_hei;
	int32_t c_wid;
	int32_t grid_size;
	const char* identifier;
	float opacity;
	int32_t px_total_offset_x;
	int32_t px_total_offset_y;
	int32_t tileset_def_uid;
	const char* tileset_rel_path;
	const char* type;
	ldtk_tile_t* auto_layer_tiles;
	int32_t auto_layer_tile_count;
	ldtk_entity_t* entities;
	int32_t entity_count;
	ldtk_tile_t* grid_tiles;
	int32_t grid_tile_count;
	const char* iid;
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
	const char* bg_rel_path;
	const char* external_rel_path;
	ldtk_field_t* fields;
	int32_t field_count;
	const char* identifier;
	const char* iid;
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
	const char* bg_color;
	//defs
	bool external_levels;
	const char* iid;
	const char* json_version;
	ldtk_level_t* levels;
	int32_t level_count;
	//toc, seems to be some top level thing, add later if relevant
	int32_t world_grid_height;
	int32_t world_grid_width;
	const char* world_layout;
	//worlds
} ldtk_map_t;

bool map_load_ldtk(const char* path, ldtk_map_t* map);
