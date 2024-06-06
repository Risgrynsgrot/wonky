const std = @import("std");
const lua = @cImport({
    @cInclude("lua.h");
    @cInclude("lauxlib.h");
    @cInclude("lualib.h");
});

pub fn scriptLoad(L: ?*lua.struct_lua_State, path: [:0]const u8) void {
    if (L) |l| {
        const status = lua.luaL_dofile(l, path);
        if (status) {
            std.debug.print("Couldn't load file: {s}\n", .{lua.lua_tolstring(l, -1, 0)});
        }
    }
}

export fn test_func() void {
    const L = lua.luaL_newstate();
    lua.luaL_openlibs(L);
    //lua.lua_pushlightuserdata(L, client->ecs);
    //lua.lua_setglobal(L, "ecs");

    scriptLoad(L, "assets/scripts/zigtest.lua");

    lua.lua_getglobal(L, "TestZig");
    if (lua.lua_istable(L, -1)) {
        lua.lua_getfield(L, -1, "testFunc");
        if (lua.lua_pcall(L, 0, 0, 0) != lua.LUA_OK) {
            _ = lua.luaL_error(L, "Error: %s\n", lua.lua_tolstring(L, -1, 0));
        }
    } else {
        std.debug.print("was not table\n", .{});
    }

    lua.lua_close(L);
}
