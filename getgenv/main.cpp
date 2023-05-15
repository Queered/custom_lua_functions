#include <lua.hpp>

static int getgenv(lua_State* L)
{
    lua_pushvalue(L, LUA_GLOBALSINDEX);
    return 1;
}

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
  
    lua_pushcfunction(L, getgenv);
    lua_setglobal(L, "getgenv");

    luaL_dostring(L, "getgenv().hello = 'world'\nprint(getgenv().hello)"); // test script lol

    lua_close(L);
    return 0;
}
