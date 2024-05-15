#include "map.h"
#include "json_object.h"
#include <json-c/json.h>
#include <stdio.h>
#include <string.h>

bool map_load_ldtk_vector(json_object* object,
						  const char* name,
						  Vector2* result) {
	json_object* array_json = json_object_object_get(object, name);
	json_object* x_json		= json_object_array_get_idx(array_json, 0);
	json_object* y_json		= json_object_array_get_idx(array_json, 1);
	result->x				= json_object_get_int(x_json);
	result->x				= json_object_get_int(y_json);
	return true;
}

bool map_load_ldtk_ts_rect(json_object* json,
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

bool map_load_ldtk_entities(json_object* entities, ldtk_layer_t* layer) {
	layer->entities =
		malloc(sizeof(*layer->entities) * layer->entity_count); //DEALLOC
	for(int i = 0; i < layer->entity_count; ++i) {
		ldtk_entity_t* entity	 = &layer->entities[i];
		json_object* entity_json = json_object_array_get_idx(entities, i);
		JSON_GET_STR(entity_json, __identifier, entity->identifier);
		map_load_ldtk_vector(entity_json, "__grid", &entity->grid);
		map_load_ldtk_vector(entity_json, "__pivot", &entity->pivot);
		//TAGS
		map_load_ldtk_ts_rect(entity_json, "__tile", &entity->tile);
		JSON_GET_STR(entity_json, __smartColor, entity->smart_color);
		JSON_GET_STR(entity_json, iid, entity->iid);
		JSON_GET(int, entity_json, width, entity->width);
		JSON_GET(int, entity_json, height, entity->height);
		JSON_GET(int, entity_json, defUid, entity->def_uid);
		map_load_ldtk_vector(entity_json, "px", &entity->px);
		//FIELD INSTANCES
		//NOT DONE
	}
	return true;
}

bool map_load_ldtk_layers(json_object* layers, ldtk_level_t* level) {
	level->layers =
		malloc(sizeof(*level->layers) * level->layer_count); //DEALLOC
	for(int i = 0; i < level->layer_count; ++i) {
		ldtk_layer_t* layer		= &level->layers[i];
		json_object* layer_json = json_object_array_get_idx(layers, i);

		JSON_GET_STR(layer_json, __identifier, layer->identifier);
		JSON_GET_STR(layer_json, __type, layer->type);
		JSON_GET(int, layer_json, __cWid, layer->c_wid);
		JSON_GET(int, layer_json, __cHei, layer->c_hei);
		JSON_GET(int, layer_json, __gridSize, layer->grid_size);
		JSON_GET(int, layer_json, __opacity, layer->opacity);
		JSON_GET(int, layer_json, __pxTotalOffsetX, layer->px_offset_x);
		JSON_GET(int, layer_json, __pxTotalOffsetY, layer->px_offset_y);
		JSON_GET(int, layer_json, __tilesetDefUid, layer->tileset_def_uid);
		JSON_GET_STR(layer_json, __tilesetRelPath, layer->tileset_rel_path);
		JSON_GET_STR(layer_json, iid, layer->iid);
		JSON_GET(int, layer_json, levelId, layer->level_id);
		JSON_GET(int, layer_json, layerDefUid, layer->layer_def_uid);
		JSON_GET(int, layer_json, pxOffsetX, layer->px_offset_x);
		JSON_GET(int, layer_json, pxOffsetY, layer->px_offset_y);
		JSON_GET(boolean, layer_json, visible, layer->visible);
		//optional rules
		//int gridcsv
		//autolayer tiles
		JSON_GET(
			int, layer_json, overrideTilesetUid, layer->override_tileset_uid);
		//grid tiles

		json_object* entities =
			json_object_object_get(layer_json, "entityInstances");
		layer->entity_count = json_object_array_length(entities);
		map_load_ldtk_entities(entities, layer);
	}

	return true;
}

bool map_load_ldtk_fields(json_object* fields, ldtk_level_t* level) {
	level->fields =
		malloc(sizeof(*level->fields) * level->field_count); //DEALLOC
	for(int i = 0; i < level->field_count; i++) {
		ldtk_field_t* field		= &level->fields[i];
		json_object* field_json = json_object_array_get_idx(fields, i);

		JSON_GET_STR(field_json, __identifier, field->identifier);
		map_load_ldtk_ts_rect(field_json, "tile", &field->tile);
		JSON_GET_STR(field_json, type, field->type);

		if(0 == strcmp(field->type, "Int")) {
			JSON_GET(int, field_json, value, field->value.value_int);
		} else if(0 == strcmp(field->type, "String")) {
			JSON_GET_STR(field_json, value, field->value.value_string);
		} else if(0 == strcmp(field->type, "Float")) {
			JSON_GET(double, field_json, value, field->value.value_float);
		} //EXPAND THIS IF NEEDED

		JSON_GET(int, field_json, defUid, field->def_uid);
	}

	return true;
}

bool map_load_ldtk_levels(json_object* levels, ldtk_map_t* map) {
	map->levels = malloc(sizeof(*map->levels) * map->level_count); //DEALLOC
	for(int i = 0; i < map->level_count; i++) {
		ldtk_level_t* level		= &map->levels[i];
		json_object* level_json = json_object_array_get_idx(levels, i);

		JSON_GET_STR(level_json, identifier, level->identifier);
		JSON_GET_STR(level_json, iid, level->iid);
		JSON_GET(int, level_json, uid, level->uid);
		JSON_GET(int, level_json, worldX, level->world_x);
		JSON_GET(int, level_json, worldY, level->world_y);
		JSON_GET(int, level_json, worldDepth, level->world_depth);
		JSON_GET(int, level_json, pxWid, level->px_wid);
		JSON_GET(int, level_json, pxHei, level->px_hei);
		JSON_GET_STR(level_json, bgRelPath, level->bg_rel_path); //THIS CRASHES BECAUSE IT'S OPTIONAL
		JSON_GET_STR(level_json, externalRelPath, level->external_rel_path);

		json_object* fields =
			json_object_object_get(level_json, "fieldInstances");
		level->field_count = json_object_array_length(fields);
		map_load_ldtk_fields(fields, level);

		json_object* layers =
			json_object_object_get(level_json, "layerInstances");
		level->layer_count = json_object_array_length(layers);
		map_load_ldtk_layers(layers, level);
	}
	return true;
}

bool map_load_ldtk(const char* path, ldtk_map_t* map) {

	json_object* root = json_object_from_file(path);
	if(!root) {
		printf("failed to load map at path: %s\n", path);
		return false;
	}

	printf("loaded map json\n");

	JSON_GET_STR(root, iid, map->iid);
	JSON_GET_STR(root, bgColor, map->bg_color);
	JSON_GET_STR(root, jsonVersion, map->json_version);
	JSON_GET_STR(root, worldLayout, map->world_layout);
	JSON_GET(int, root, worldGridWidth, map->world_grid_width);
	JSON_GET(int, root, worldGridHeight, map->world_grid_height);
	JSON_GET(boolean, root, externalLevels, map->external_levels);

	json_object* levels = json_object_object_get(root, "levels");
	map->level_count = json_object_array_length(levels);
	map_load_ldtk_levels(levels, map);

	json_object_put(root);
	return true;
}
