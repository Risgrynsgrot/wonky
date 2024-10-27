#include "scripting.h"
#include "entity.h"
#include "gameworld.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

//d double
//i int
//u userdata
//l lightuserdata
//s string
//b bool
void call_lua_event(lua_State* L,
					gameworld_t* world,
					const char* event_name,
					entity_t* entity,
					const char* signature,
					...) {

	const char* type = entity_get_type(&world->entities, *entity);
	lua_getglobal(L, "Entity");
	lua_getfield(L, -1, "call_event");
	lua_pushstring(L, type);
	lua_pushstring(L, event_name);

	va_list args;
	va_start(args, signature);
	int arg_count = 0;

	while(*signature) {
		switch(*signature) {
		case 'd':
			lua_pushnumber(L, va_arg(args, double));
			break;
		case 'i':
			lua_pushnumber(L, va_arg(args, double));
			break;
		case 'u':
			lua_pushlightuserdata(L, va_arg(args, void*));
			break;
		case 's':
			lua_pushstring(L, va_arg(args, const char*));
			break;
		case 'b':
			lua_pushboolean(L, va_arg(args, int));
			break;
		case '>':
			signature++;
			goto endwhile;
		default:
			assert(false && "invalid type in call_lua_event");
			break;
		}
		signature++;
		arg_count++;
	}
endwhile: {

	int return_count = strlen(signature);
	if(lua_pcall(L, arg_count, return_count, 0) != 0) {
		printf("error running event %s", event_name);
	}

	int return_stack_pos = -return_count;
	while(*signature) {
		switch(*signature) {
		case 'd':
			*va_arg(args, double*) = lua_tonumber(L, return_stack_pos);
			break;
		case 'i':
			*va_arg(args, int*) = (int)lua_tonumber(L, return_stack_pos);
			break;
		case 'u':
			*va_arg(args, void**) = lua_touserdata(L, return_stack_pos);
			break;
		case 's':
			*va_arg(args, const char**) = lua_tostring(L, return_stack_pos);
			break;
		case 'b':
			*va_arg(args, bool*) = lua_toboolean(L, return_stack_pos);
			break;
		default:
			assert(false && "invalid type in call_lua_event return types");
			break;
		}
		signature++;
		return_stack_pos++;
	}
	va_end(args);
}
}

void event_call_on_create(lua_State* L, gameworld_t* world, entity_t* entity) {
	call_lua_event(L, world, "on_create", entity, "u", entity);
}
