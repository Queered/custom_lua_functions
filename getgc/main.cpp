#include <lua.hpp>
#include <vector>

int getgc(lua_State* L) {
    bool include_tables = luaL_optinteger(L, 1, 0);

    std::vector<void*> results;
    lua_State* L1 = lua_newthread(L);
    lua_gc(L1, LUA_GCCOLLECT, 0);
    lua_pushnil(L1);
    while (lua_next(L1, LUA_REGISTRYINDEX)) {
        if (lua_isfunction(L1, -1) || lua_isuserdata(L1, -1) || (include_tables && lua_istable(L1, -1))) {
            results.push_back(lua_topointer(L1, -1));
        }
        lua_pop(L1, 1);
    }

    lua_newtable(L);
    for (size_t i = 0; i < results.size(); i++) {
        lua_pushinteger(L, i + 1);
        lua_pushlightuserdata(L, results[i]);
        lua_settable(L, -3);
    }

    return 1;
}

int main() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_pushcfunction(L, getgc);
    lua_setglobal(L, "getgc");

    luaL_dostring(L, "local gc_values = getgc(true)\nfor i, v in ipairs(gc_values) do\n    print(i, v)\nend"); // test script for your brand NEW getgc! enjoy

    lua_close(L);
    return 0;
}
