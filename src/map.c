#include "map.h"
#include <json-c/json.h>
#include <stdio.h>

#define JSON_GET_STRING(ROOT, JSON_NAME, RESULT) \
do { \
	json_object* JSON_NAME##_json = json_object_object_get(ROOT, #JSON_NAME);\
	RESULT = json_object_get_string(JSON_NAME##_json);\
} while(0)

#define JSON_GET_INT(ROOT, JSON_NAME, RESULT) \
do { \
	json_object* JSON_NAME##_json = json_object_object_get(ROOT, #JSON_NAME);\
	RESULT = json_object_get_int(JSON_NAME##_json);\
} while(0)

#define JSON_GET_FLOAT(ROOT, JSON_NAME, RESULT) \
do { \
	json_object* JSON_NAME##_json = json_object_object_get(ROOT, #JSON_NAME);\
	RESULT = json_object_get_float(JSON_NAME##_json);\
} while(0)

#define JSON_GET_BOOL(ROOT, JSON_NAME, RESULT) \
do { \
	json_object* JSON_NAME##_json = json_object_object_get(ROOT, #JSON_NAME);\
	RESULT = json_object_get_boolean(JSON_NAME##_json);\
} while(0)


bool map_load_ldtk_levels(json_object* root, ldtk_map_t* map) {
	return true;
}

bool map_load_ldtk(const char* path, ldtk_map_t* map) {

	json_object* root = json_object_from_file(path);
	if(!root) {
		printf("failed to load map at path: %s\n", path);
		return false;
	}

	JSON_GET_STRING(root, iid, map->iid);
	JSON_GET_STRING(root, bgColor, map->bg_color);
	JSON_GET_STRING(root, jsonVersion, map->json_version);
	JSON_GET_STRING(root, worldLayout, map->world_layout);
	JSON_GET_INT(root, worldGridWidth, map->world_grid_width);
	JSON_GET_INT(root, worldGridHeight, map->world_grid_height);
	JSON_GET_BOOL(root, externalLevels, map->external_levels);

	map_load_ldtk_levels(root, map);

	json_object_put(root);
	return true;
}
