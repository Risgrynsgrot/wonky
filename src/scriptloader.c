#include "scriptloader.h"
#include "lualib.h"
#include "lauxlib.h"

lua_State* script_lua_init(void) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	return L;
}
void script_lua_close(lua_State* L) {
	lua_close(L);
}

void script_load(lua_State* L, const char* path) {
	int status;
	status = luaL_loadfile(L, path);
	if(status) {
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
	}
	return;
}
