#include "components.h"
#include "entity.h"
#include "gameworld.h"
#include "rendersystem.h"
#include "scriptloader.h"
#include "serializer.h"
#include <assert.h>
#include <lauxlib.h>
#include <lua.h>
#include <stdio.h>
#include <string.h>

//ECS_COMPONENTS_TYPE_ITER(DECL_COMPONENT_IDS, void)
//ecs_component_string_t ecs_component_strings[COMPONENT_COUNT];
void (*component_serializers[])(serializer_t* serializer, void* data) = {
	ECS_COMPONENTS_TYPE_ITER(DECL_COMPONENT_SERIALIZERS, void)};

//int ecs_component_string_count;

//TODO(risgrynsgrot): Add ecs_lua_add_component_functions that maps component id
//to lua serialize function

void ser_position(serializer_t* ser, void* data) {
	comp_position_t* position = (comp_position_t*)data;
	ser->ser_vec2(ser, &position->value, "value");
	ser->ser_vec2(ser, &position->grid_pos, "grid_pos");
	ser->ser_int(ser, &position->layer, "layer");
}

void ser_rotation(serializer_t* ser, void* data) {
	comp_rotation_t* rotation = (comp_rotation_t*)data;
	ser->ser_float(ser, &rotation->angle, "angle");
}

void ser_velocity(serializer_t* ser, void* data) {

	comp_velocity_t* velocity = (comp_velocity_t*)data;
	ser->ser_vec2(ser, &velocity->value, "value");
}

void ser_input(serializer_t* ser, void* data) {
	comp_input_t* input = (comp_input_t*)data;
	ser->ser_int(ser, &input->input_id, "input_id");
	ser->ser_vec2(ser, &input->direction, "direction");
	ser->ser_bool(ser, &input->interact, "interact");
	ser->ser_bool(ser, &input->open_inventory, "open_directory");
}

void ser_area_box(serializer_t* ser, void* data) {
	comp_area_box_t* area_box = (comp_area_box_t*)data;
	ser->ser_float(ser, &area_box->width, "width");
	ser->ser_float(ser, &area_box->height, "height");
	ser->ser_float(ser, &area_box->offset_x, "offset_x");
	ser->ser_float(ser, &area_box->offset_y, "offset_y");
	//ecs_id_t overlaps[MAX_OVERLAP_COUNT];
	//int overlapCount;
}

void ser_col_box(serializer_t* ser, void* data) {
	comp_col_box_t* col_box = (comp_col_box_t*)data;
	ser->ser_float(ser, &col_box->width, "width");
	ser->ser_float(ser, &col_box->height, "height");
	ser->ser_float(ser, &col_box->offset_x, "offset_x");
	ser->ser_float(ser, &col_box->offset_y, "offset_y");
}

void ser_draw_sprite(serializer_t* ser, void* data) {
	comp_draw_sprite_t* draw_sprite = (comp_draw_sprite_t*)data;
	ser->ser_float(ser, &draw_sprite->width, "width");
	ser->ser_float(ser, &draw_sprite->height, "height");
	ser->ser_float(ser, &draw_sprite->offset_x, "offset_x");
	ser->ser_float(ser, &draw_sprite->offset_y, "offset_y");
	ser->ser_bool(ser, &draw_sprite->visible, "visible");
	ser->ser_color(ser, &draw_sprite->color, "color");
	//ser->ser_Texture2D texture;
}

void ser_draw_box(serializer_t* ser, void* data) {
	comp_draw_box_t* draw_box = (comp_draw_box_t*)data;
	ser->ser_float(ser, &draw_box->width, "width");
	ser->ser_float(ser, &draw_box->height, "height");
	ser->ser_float(ser, &draw_box->offset_x, "offset_x");
	ser->ser_float(ser, &draw_box->offset_y, "offset_y");
	ser->ser_bool(ser, &draw_box->visible, "visible");
	ser->ser_color(ser, &draw_box->color, "color");
}

void ser_draw_circle(serializer_t* ser, void* data) {
	comp_draw_circle_t* draw_circle = (comp_draw_circle_t*)data;
	ser->ser_float(ser, &draw_circle->radius, "radius");
	ser->ser_float(ser, &draw_circle->offset_x, "offset_x");
	ser->ser_float(ser, &draw_circle->offset_y, "offset_y");
	ser->ser_bool(ser, &draw_circle->visible, "visible");
	ser->ser_color(ser, &draw_circle->color, "color");
}

void ser_mover(serializer_t* ser, void* data) {
	comp_mover_t* mover = (comp_mover_t*)data;
	ser->ser_float(ser, &mover->movement_speed, "movement_speed");
}


//void ecs_component_register_string(ecs_component_string_t value) {
//	int i						 = ecs_component_string_count;
//	ecs_component_string_t* dest = &ecs_component_strings[i];
//	dest->id					 = value.id;
//	strcpy(dest->name, value.name);
//}

//void* ecs_add_component_string(ecs_t* ecs, ecs_id_t entity, const char* value) {
//	for(int i = 0; i < ecs_component_string_count; i++) {
//		if(strcmp(value, ecs_component_strings[i].name) == 0) {
//			void* result =
//				ecs_add(ecs, entity, ecs_component_strings[i].id, NULL);
//			return result;
//		}
//	}
//	assert(false && "tried to add nonexistent component, did you misspell?");
//	return NULL;
//}

//bool ecs_string_to_componentid(ecs_id_t* out_result, const char* value) {
//	for(int i = 0; i < ecs_component_string_count; i++) {
//		if(strcmp(value, ecs_component_strings[i].name) != 0) {
//			continue;
//		}
//		*out_result = ecs_component_strings[i].id;
//		return true;
//	}
//	return false;
//}

#define LUA_SET_COMP(lc, uc, i, ...)                                           \
	int ecs_lua_set_##lc(serializer_t* ser, entity_t entity) {                 \
		gameworld_t* world = script_get_userdata(ser->ser.lua.L, "world");     \
		comp_##lc##_t* lc  = world->entities.##lc##_a[entity.id];              \
		ser_##lc(ser, lc);                                                     \
		return 0;                                                              \
	}

//TODO(risgrynsgrot) Create table and return it as part of getting comp
#define LUA_GET_COMP(lc, uc, i, ...)                                           \
	int ecs_lua_get_##lc(serializer_t* ser, entity_t entity) {                 \
		gameworld_t* world = script_get_userdata(ser->ser.lua.L, "world");     \
		comp_##lc##_t* lc  = &world->entities.lc##_a[entity.id];               \
		lua_newtable(ser->ser.lua.L);                                          \
		lua_pushnumber(ser->ser.lua.L, i);                                     \
		lua_setfield(ser->ser.lua.L, -2, "type");                              \
		printf("stack before ser: \n");                                        \
		script_dumpstack(ser->ser.lua.L);                                      \
		ser_##lc(ser, lc);                                                     \
		printf("stack after ser: \n");                                         \
		script_dumpstack(ser->ser.lua.L);                                      \
		return 1;                                                              \
	}

//ECS_COMPONENTS_TYPE_ITER(LUA_SET_COMP, void)
//ECS_COMPONENTS_TYPE_ITER(LUA_GET_COMP, void)

int ecs_lua_add_sprite(lua_State* L) {
	entity_t* entity = (entity_t*)lua_touserdata(L, -2);
	const char* path = lua_tostring(L, -1);
	lua_pop(L, 1);
	gameworld_t* world = script_get_userdata(L, "world");
	render_load_sprite(world, path, *entity);
	return 0;
}

int ecs_lua_set_component(lua_State* L) {
	printf("setting component...\n");
	entity_t* entity = (entity_t*)lua_touserdata(L, -2);
	if(lua_istable(L, -1)) { //the table with the component info
		lua_getfield(L, -1, "type");
		if(lua_isnumber(L, -1)) {
			component_types_e type = lua_tonumber(L, -1);
			lua_pop(L, 1);
			printf("component_type: %d\n", type);
			serializer_t ser   = new_reader_lua((ser_lua_t){.L = L});
			gameworld_t* world = script_get_userdata(L, "world");
			void* component =
				entity_get_component(&world->entities, *entity, type);
			component_serializers[type](&ser, component);
		} else {
			printf("not a component number\n");
		}
	} else {
		printf("not a table\n");
	}
	return 0;
}

//TODO(risgrynsgrot) Change type getting to use ints, to reduce strcmp
int ecs_lua_get_component(lua_State* L) {
	entity_t* entity = (entity_t*)lua_touserdata(L, -2);
	if(lua_isnumber(L, -1)) {
		component_types_e type = lua_tonumber(L, -1);
		serializer_t ser	   = new_writer_lua((ser_lua_t){.L = L});

		gameworld_t* world = script_get_userdata(L, "world");
		void* component = entity_get_component(&world->entities, *entity, type);

		lua_newtable(L);
		lua_pushnumber(L, type);
		lua_setfield(L, -2, "type");

		component_serializers[type](&ser, component);

	} else {
		printf("not a string\n");
	}

	return 1;
}

int lua_add_trait(lua_State* L) {
	if(!lua_isuserdata(L, -2)) {
		printf("add trait called with first parameter not a entity\n");
		return 0;
	}
	if(!lua_isnumber(L, -1)) {
		printf("add trait called with second parameter not a number\n");
		return 0;
	}
	entity_t* entity   = (entity_t*)lua_touserdata(L, -2);
	trait_types_e type = lua_tonumber(L, -1);

	gameworld_t* world = script_get_userdata(L, "world");
	trait_entity_add(&world->traits, type, *entity);

	return 1;
}

int lua_remove_trait(lua_State* L) {
	if(!lua_isuserdata(L, -2)) {
		printf("remove trait called with first parameter not a entity\n");
		return 0;
	}
	if(!lua_isnumber(L, -1)) {
		printf("remove trait called with second parameter not a number\n");
		return 0;
	}
	entity_t* entity   = (entity_t*)lua_touserdata(L, -2);
	trait_types_e type = lua_tonumber(L, -1);

	gameworld_t* world = script_get_userdata(L, "world");
	trait_entity_remove(&world->traits, type, *entity);

	return 1;
}

//move all this to some lua scripting file
static const struct luaL_Reg ecs_functions[] = {
	{"get", ecs_lua_get_component},
	{"set", ecs_lua_set_component},
	{ NULL,				  NULL}
};

static const struct luaL_Reg trait_functions[] = {
	{	 "add",	lua_add_trait},
	{"remove", lua_remove_trait},
	  {	   NULL,			 NULL}
};

static const struct luaL_Reg asset_functions[] = {
	{"add_sprite", ecs_lua_add_sprite},
	{		 NULL,			   NULL}
};

void ecs_lua_register_module(lua_State* L) {
	luaL_register(L, "Comp", ecs_functions);
	luaL_register(L, "Asset", asset_functions);
	luaL_register(L, "Trait", trait_functions);
}

#define DECL_COMPONENT_SWITCH(lc, uc, i, ...)                                  \
	case COMPONENT_##uc:                                                       \
		return &entities->lc##_a[entity.id];

void* entity_get_component(entities_t* entities,
						   entity_t entity,
						   component_types_e component) {
	switch(component) {
		ECS_COMPONENTS_TYPE_ITER(DECL_COMPONENT_SWITCH, void)
	default:
		assert(false);
		return NULL;
	}
}

void lua_register_component_enum(lua_State* L) {
	lua_newtable(L);
	ECS_COMPONENTS_TYPE_ITER(DECL_LUA_ENUM, void)
	script_dumpstack(L);
	lua_setglobal(L, "Comp");
}

void lua_register_traits_enum(lua_State* L) {
	lua_newtable(L);
	TRAITS_TYPE_ITER(DECL_LUA_ENUM, void)
	script_dumpstack(L);
	lua_setglobal(L, "Trait");
}
