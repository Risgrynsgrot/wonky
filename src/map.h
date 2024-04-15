#pragma once
#include <raymath.h>
#include <stdbool.h>
#include <stdint.h>

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
	int32_t tilesetUid;
} ldtk_ts_rect_t;

typedef struct ldtk_tile {
	float a;
	int f;
	Vector2 px;
	Vector2 src;
	int t;
} ldtk_tile_t;

typedef struct ldtk_field_instance_value {
	union {
		int value_int;
		float value_float;
		bool value_bool;
		char value_string[256]; //If this causes issues, expand string
		ldtk_ts_rect_t value_tile;
		ldtk_grid_point_t value_point;
		ldtk_entity_ref_t value_entity_ref;
	};

} ldtk_field_instance_value_t;

typedef struct ldtk_field_instance {
	const char* identifier;
	ldtk_ts_rect_t tile;
	const char* type;
	ldtk_field_instance_value_t value;
	int def_uid;
} ldtk_field_instance_t;

typedef struct ldtk_entity_instance {
	Vector2 grid;
	const char* identifier;
	Vector2 pivot;
	const char* smart_color;
	const char** tags;
	int tag_count;
	ldtk_tile_t tile;
	int world_x;
	int world_y;
	int def_uid;
	ldtk_field_instance_t* field_instances;
	int field_instance_count;
	int width;
	int height;
	const char* iid;
	Vector2 px;
} ldtk_entity_instance_t;

typedef struct ldtk_layer_instance {
	int c_hei;
	int c_wid;
	int grid_size;
	const char* identifier;
	float opacity;
	int px_total_offset_x;
	int px_total_offset_y;
	int tileset_def_uid;
	const char* tileset_rel_path;
	const char* type;
	ldtk_tile_t* auto_layer_tiles;
	int auto_layer_tile_count;
	ldtk_entity_instance_t* entity_instances;
	int entity_instance_count;
	ldtk_tile_t* grid_tiles;
	int grid_tile_count;
	const char* iid;
	int* int_grid_csv;
	int int_grid_count;
	int layer_def_uid;
	int level_id;
	int override_tileset_uid;
	int px_offset_x;
	int px_offset_y;
	bool visible;
} ldtk_layer_instance_t;

typedef struct ldtk_level {
	const char* bg_rel_path;
	const char* external_rel_path;
	ldtk_field_instance_t* field_instances;
	int field_instance_count;
	const char* identifier;
	const char* iid;
	ldtk_layer_instance_t* layer_instances;
	int layer_instance_count;
	int px_hei;
	int px_wid;
	int uid;
	int world_depth;
	int world_x;
	int world_y;
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
	int level_count;
	//toc, seems to be some top level thing, add later if relevant
	int world_grid_height;
	int world_grid_width;
	const char* world_layout;
	//worlds
} ldtk_map_t;
