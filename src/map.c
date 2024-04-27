#include "map.h"
#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

bool map_load_ldtk_tile(json_object* json,
						const char* name,
						ldtk_ts_rect_t* tile) {
	json_object* tile_json = json_object_object_get(json, name);
	JSON_GET(int, tile_json, x, tile->x);
	JSON_GET(int, tile_json, y, tile->y);
	JSON_GET(int, tile_json, w, tile->w);
	JSON_GET(int, tile_json, h, tile->h);
	JSON_GET(int, tile_json, tilesetUid, tile->tileset_uid);
	return true;
}

//bool map_load_ldtk_layer_instances(json_object* layer_instances,
								   //ldtk_level_t* level) {

//}

bool map_load_ldtk_field_instances(json_object* field_instances,
								   ldtk_level_t* level) {
	for(int i = 0; i < level->field_instance_count; i++) {
		ldtk_field_instance_t* fi = &level->field_instances[i];
		json_object* fi_json = json_object_array_get_idx(field_instances, i);

		JSON_GET(string, fi_json, __identifier, fi->identifier);
		map_load_ldtk_tile(fi_json, "tile", &fi->tile);
		JSON_GET(string, fi_json, type, fi->type);

		if(0 == strcmp(fi->type, "Int")) {
			JSON_GET(int, fi_json, value, fi->value.value_int);
		} else if(0 == strcmp(fi->type, "String")) {
			JSON_GET(string, fi_json, value, fi->value.value_string);
		} else if(0 == strcmp(fi->type, "Float")) {
			JSON_GET(double, fi_json, value, fi->value.value_float);
		} //EXPAND THIS IF NEEDED

		JSON_GET(int, fi_json, defUid, fi->def_uid);
	}

	return true;
}

bool map_load_ldtk_levels(json_object* levels, ldtk_map_t* map) {

	for(int i = 0; i < map->level_count; i++) {
		ldtk_level_t* level		= &map->levels[i];
		json_object* level_json = json_object_array_get_idx(levels, i);

		JSON_GET(string, level_json, identifier, level->identifier);
		JSON_GET(string, level_json, iid, level->iid);
		JSON_GET(int, level_json, uid, level->uid);
		JSON_GET(int, level_json, worldX, level->world_x);
		JSON_GET(int, level_json, worldY, level->world_y);
		JSON_GET(int, level_json, worldDepth, level->world_depth);
		JSON_GET(int, level_json, pxWid, level->px_wid);
		JSON_GET(int, level_json, pxHei, level->px_hei);
		JSON_GET(string, level_json, bgRelPath, level->bg_rel_path);
		JSON_GET(string, level_json, externalRelPath, level->external_rel_path);

		json_object* field_instances =
			json_object_object_get(level_json, "fieldInstances");
		JSON_ARR_LEN(level_json, field_instances, level->field_instance_count);
		map_load_ldtk_field_instances(field_instances, level);
		//"layerInstances": [
	}
	return true;
}

bool map_load_ldtk(const char* path, ldtk_map_t* map) {

	json_object* root = json_object_from_file(path);
	if(!root) {
		printf("failed to load map at path: %s\n", path);
		return false;
	}

	JSON_GET(string, root, iid, map->iid);
	JSON_GET(string, root, bgColor, map->bg_color);
	JSON_GET(string, root, jsonVersion, map->json_version);
	JSON_GET(string, root, worldLayout, map->world_layout);
	JSON_GET(int, root, worldGridWidth, map->world_grid_width);
	JSON_GET(int, root, worldGridHeight, map->world_grid_height);
	JSON_GET(boolean, root, externalLevels, map->external_levels);

	json_object* levels = json_object_object_get(root, "levels");
	JSON_ARR_LEN(root, levels, map->level_count);
	map_load_ldtk_levels(levels, map);

	json_object_put(root);
	return true;
}
